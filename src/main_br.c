/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "device.h"
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "interrupt.h"
#include "common.h"
#include "time.h"
#include "led_controller.h"

static TempMonitor m_monitor = {INIT /*MonitorStatus*/, SENSOR_A/*ActiveSensor*/};
 
/*Define the set of relevant peripherals*/
ADC_TypeDef * ADC1 = &ADC1_dev;
GPIO_TypeDef * GPIO1 = &GPIO1_dev;
GPIO_TypeDef * GPIO2 = &GPIO2_dev;
GPIO_TypeDef * GPIO3 = &GPIO3_dev;
GPIO_TypeDef * GPIO4 = &GPIO4_dev;
GPIO_TypeDef * GPIO5 = &GPIO5_dev;
GPIO_TypeDef * GPIOC = &GPIOC_dev;
I2C_TypeDef * I2C1 = &I2C1_dev;
//InterruptDef *adc_it_ptr = &m_adc_it;



//Declare Interrupt Callbacks
//
static void adc_convert_callback(){
	
	//Read Temperature Value from ADC;
	uint16_t Tvalue = ADC_GetConvertionValue(ADC1);
	//Update Current Temperature 
	update_adc_value(Tvalue);
	//Clean up Interrupt Flag; 
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}

static void i2c_recv_callback(){
       //Read Revision from EEPROM 
       //Clean up Interrupt Flag
       //unused
}
/*
 * Pheripheral Configuration Parameters 
 * 
 * GPIO: 
 *	GPIO1 = Temperature Sensor Input Analog. 
 *	GPIO2,3,4 = Digital Output. 
*/ 
//GPIO1 
GPIO_InitTypeDef adc_in_gpio_conf = {.GPIO_pin = GPIO_1, 
				  .GPIO_Mode = GPIO_MODE_AN, 
				  .GPIO_PuPd = GPIO_PuPd_NOPULL
				 };
//GPIO2,3,4
GPIO_InitTypeDef output_gpio_conf = {
		.GPIO_pin = GPIO_2 || GPIO_3 || GPIO_4,
		.GPIO_Mode = GPIO_Mode_OUT, 
		.GPIO_PuPd = GPIO_PuPd_DOWN
		};

/*
 * I2C1: Master Configuration 
 *	 Characateristics 
 *	 - Clock generation
 *	 - Start and Stop generation
 *	 - Status flags:
 *		- Transmitter/Receiver mode flag
 *		- End-of-Byte transmission flag
 *		- I2C busy flag
 *
 *
 * */
#define GPIO_SCL                  GPIO_5
#define GPIO_SDA                  GPIO_6 
GPIO_InitTypeDef i2c_gpio_conf = {
	.GPIO_pin = GPIO_SCL,
	.GPIO_Mode = GPIO_Mode_AF,
	.GPIO_PuPd  = GPIO_PuPd_UP
};

I2C_InitTypeDef i2c_conf  = {
	.I2C_ClockSpeed = 5000, //400kHz (fast Mode)
	.I2C_Mode = I2C_Mode_I2C,//
	.I2C_DutyCycle = I2C_DutyCycle_2,
	.I2C_addrs = 0x00, // Master Node does not need address
	.I2C_ack = I2C_Ack_Enable
};


/*
 * ADC:	
 * Device:
 *	- Interrupt generation at the end of conversion
 *	- 12bits resolution 
 *	- Indeally (configurable DMA data storage)
 *	- Vref: 5v
 *	- DC input range: V_REF– ≤V IN ≤V_REF+
 *
 *
 *	ADC should convert two sensors with the following
 *	Characteristics
 *	range :-10C to 110C
 *	Sensor-A 1C/digit,  # values = 120
 *	Sensor-B 0.1C/digit # values = 1200
 *	ADC Maximal Resoulution
 *	2^n_bits = 1200, 
 *	2^10 = 1024
 *	2^12 = 4090 values  
*/ 
ADC_InitTypeDef adc_conf = {.Resolution = r12Bits, 
			    .DataAlign = BigIndian
			   };

//Defining interrupts 
//
InterruptDef m_adc_it = {.it_id = ADC1_DEV, 
			 .callback = adc_convert_callback,
			 .mux = PTHREAD_MUTEX_INITIALIZER,
			 .interval = 1, // 1 = 100us 
			 .status = IT_DISABLE 
			};

InterruptDef *adc_it_ptr = &m_adc_it;


uint8_t update_config_data(void) {	
	clock_t start_time = clock();
	HardwareRevision SensorConf;
	//Send Start Condition
	I2C_GenerateStart(I2C1, ENABLE);
	//SEND Device Address 
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	//Receive Data 
	I2C_ReceiveData(I2C1);

	//wait for data to be received in a pooling state. 
	//I2C_IT_BTF
	while( !I2C_GetItStatus(I2C1, (I2C_IT_BTF & I2C_IT_TIMEOUT ))) {}
	       //Verify there was not errors during transmission.
	if (I2C_GetItStatus(I2C1, I2C_IT_TIMEOUT) == true 
	     && I2C_GetItStatus(I2C1, I2C_IT_BTF)== false ) {
		printf("-1\n");
		exit(0);
		return -1;
	}
	
	memcpy((void*)&SensorConf, (const void*)&I2C1->DR, sizeof(HardwareRevision));
	
	//update config data
	if (SensorConf.Rev == 0x0){
		m_monitor.ActiveSensor = SENSOR_A;
	}else if (SensorConf.Rev == 0x1) {
		m_monitor.ActiveSensor = SENSOR_B;
	}else{
	  //Invalid Sensor Data 
	  snprintf(app_message, sizeof(app_message), 
		   "[ERROR] \t %s  Invalid SensorData", 
		   get_timestamp()
		   );
	  zeiss_printf(app_message);
	  return -1;
	} 
	I2C_ClearITFlag(I2C1, I2C_IT_BTF);
	clock_t end_time = clock();
	double time_passed = (double) 100 * (end_time - start_time) / CLOCKS_PER_SEC; 
	
	snprintf(app_message, sizeof(app_message), 
		 "[INFO] \t Update Data from I2C takes %.6f us\n",
		 time_passed
		 );
	zeiss_printf(app_message);
	return 0;
}

uint8_t report_temperature_status(){
	clock_t start_time = clock();
	double Tvalue = (double) get_adc_value();
	//Process 
	set_all_leds(0);
	if (m_monitor.ActiveSensor == SENSOR_A) Tvalue = Tvalue / 10;
	if (Tvalue >= 105 || Tvalue <= 5) set_led(2,1); //Critical State
	else if (Tvalue >=85 && Tvalue < 105)set_led(1,1); //Warining State
	else if (Tvalue > 5 && Tvalue < 85) set_led(0,1); // Normal State 
	clock_t end_time = clock();
	
	double time_passed = (double) 100 * (end_time - start_time) / CLOCKS_PER_SEC; 
	
	snprintf(app_message, sizeof(app_message), 
		 "[INFO] \t Update Temperature DATA takes %.6f us Temp: %f",
		 time_passed,
		 Tvalue
		 );

	zeiss_printf(app_message);
	return 0;


}
int monitor_init(void){
    //Configure GPIO for I2C-SCL
	GPIO_Init(GPIO4, &i2c_gpio_conf);
    //Configure GPIO for I2C_SDA
	i2c_gpio_conf.GPIO_pin = GPIO_SDA;
	GPIO_Init(GPIO5, &i2c_gpio_conf);
    //Initialize I2C 
	I2C_Init(I2C1, &i2c_conf);
   //Configure GPIO1 For ADC
	GPIO_Init(GPIO1, &adc_in_gpio_conf);
   //Initialize ADC
	ADC_Init(ADC1, &adc_conf);
   //Initialize output GPIOs 
	GPIO_Init(GPIOC, &output_gpio_conf);
   /*initializing ADC Interrupt*/
	IT_Init(&adc_it_ptr);
   return 0;
}


//Logic
int monitor_main_loop(void){
    int ret = 0;
    switch (m_monitor.Status) {
       case INIT:
       if( (ret = monitor_init()) !=0) {
			printf("1\n");
            SetMonitorNextState(&m_monitor, ON_ERROR);
             return -1;
         }
		 printf("Step 1\n");
        SetMonitorNextState(&m_monitor, PULL_CONF);
        break;
        case PULL_CONF:
        if((ret = update_config_data()) !=0){
            SetMonitorNextState(&m_monitor, ON_ERROR);
            return -1; 
        }
        SetMonitorNextState(&m_monitor, REPORTING);
        break;
        case REPORTING:
        if ((ret = report_temperature_status()) !=0){
            SetMonitorNextState(&m_monitor, ON_ERROR);
            return -1;
        }
        SetMonitorNextState(&m_monitor, PULL_CONF);
        break;
        case ON_ERROR:
        	zeiss_printf("[ERROR] \t Monitor Main Loop General Error \n");
        	return -1;
		default:
			break;
    }
    return 0;
}

int main(void){
 //IT_Init(&adc_it_ptr);
 
    // Initialize shared memory
    if (init_shared_memory() != 0) {
        return 1;
    }
    
 while(true){

	monitor_main_loop();
	sleep(0.5);
 }
 return 0;
}
