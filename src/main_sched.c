
/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "hal.h"
#include "led_controller.h"
#include "edf_sched.h"

//defined values
#define GPIO_SCL                  GPIO_5
#define GPIO_SDA                  GPIO_6
#define ADC_RESOLUTION_8BITS 0x08
#define ADC_RESOLUTION_10BITS 0x0A
#define ADC_RESOLUTION_12BITS 0x0C
#define ADC_LITTLE_ENDIAN 0x0
#define ADC_BIG_ENDIAN 0x1
#define EEPROM_ADDR ((uint8_t)0x70)

//Static Variables
static TempMonitor m_monitor = {INIT /*MonitorStatus*/, SENSOR_A/*ActiveSensor*/};
// Static Functions

static void adc_convert_callback(void){
	//Read Temperature Value form ADC
	
}

static int adc_init(void) {
	//Configure GPIO1 For ADC
	 HAL_ADC_Init(_ADC1, ADC_RESOLUTION_12BITS, ADC_LITTLE_ENDIAN);
	 HAL_ADC_IT_Init(_ADC1, adc_convert_callback, 100);
   return 0;
};

static int adc_pull_temp(void) {
   if (HAL_ADC_GetITPendingBit(_ADC1,_ADC_IT_EOC)){
	  	//Read Temperature Value form ADC
		 uint16_t Tvalue = HAL_ADC_GetConvertionValue(_ADC1);
		 //Update Current Temperature
		 update_adc_value(Tvalue); 
		//Clean the Interrupt Flag; 
		HAL_ADC_ClearITPendingBit(_ADC1, _ADC_IT_EOC);
   }
   return 0;
};


static
int pull_config_data(void){
    clock_t start_time = clock();
    HardwareRevision SensorConf;
    HardwareRevision* sensor_conf_ptr = &SensorConf;
    //Set Start Flag
    HAL_I2CGenerateStart(_I2C1,ENABLE);
    //SEND Remote Device Address 
    HAL_I2C_Send7bitAddress(_I2C1, EEPROM_ADDR, _I2C_Direction_Transmitter);
    //Receive Data 
    HAL_I2C_ReceiveData(_I2C1);
    //Wait until all data is received
    while(!HAL_I2C_GetItStatus(_I2C1, (I2C_IT_BTF & I2C_IT_TIMEOUT ))){}
    if (HAL_I2C_GetItStatus(_I2C1, I2C_IT_TIMEOUT) == true 
	     && HAL_I2C_GetItStatus(_I2C1, I2C_IT_BTF)== false ) {
            zeiss_printf("[ERROR] \t i2c data corrupted or timeout");
            return -1;
         }
    //get data from HAL
    HAL_I2C_GetReceivedData(_I2C1, (void*)&sensor_conf_ptr,sizeof(HardwareRevision));
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
    HAL_I2C_ClearITFlag(_I2C1,I2C_IT_BTF);
    clock_t end_time = clock();
    double time_passed = (double) 100 * (end_time - start_time) / CLOCKS_PER_SEC; 
    	snprintf(app_message, sizeof(app_message), 
		 "[INFO] \t Update Data from I2C takes %.6f s",
		 time_passed
		 );
//	zeiss_printf(app_message);
    return 0;
}

static int monitor_init(void) {
	//Configure GPIO for I2C-SCL
       HAL_GPIO_Init(_GPIO5,  GPIO_SCL, _GPIO_MOD_AF,_GPIO_PMOD_NOPULL);
    //Configure GPIO for I2C-SDA
	HAL_GPIO_Init(_GPIO6,  GPIO_SDA, _GPIO_MOD_AF,_GPIO_PMOD_NOPULL);
	//Initialize I2C 
	HAL_I2CInit(_I2C1, 5000, _I2C_Mode_I2C, _I2C_DutyCycle_2, 0x00, _I2C_Ack_Enable);
	//Initialize output GPIOs 
       uint32_t gpio_pins = (GPIO_7 || GPIO_8 || GPIO_9);
       HAL_GPIOBus_Init(_GPIOC, gpio_pins, _GPIO_MOD_OUT,  _GPIO_PMOD_PULLDOWN);
	//Update Monitor
	SetMonitorNextState(&m_monitor, PULL_CONF);
	return 0;
};


uint8_t report_temperature_status(){
    clock_t start_time = clock();
    double Tvalue = (double) get_adc_value();
    set_all_leds(0);
    if (m_monitor.ActiveSensor == SENSOR_A) Tvalue = Tvalue / 10;
    if (Tvalue >= 105 || Tvalue <= 5) set_led(2,1); //Critical State
	else if (Tvalue >=85 && Tvalue < 105)set_led(1,1); //Warining State
	else if (Tvalue > 5 && Tvalue < 85) set_led(0,1); // Normal State 
    clock_t end_time = clock();
    double time_passed = (double) 100 * (end_time - start_time) / CLOCKS_PER_SEC; 
		snprintf(app_message, sizeof(app_message), 
		 "[INFO] \t Pulling temperature DATA takes %.6f s Temp: %f",
		 time_passed,
		 Tvalue
		 );
//	zeiss_printf(app_message);
	return 0;
}

static int monitor_temp_process(void) {
	int ret = 0;
	switch (m_monitor.Status) {
		case INIT:
		//this should not be called from here. 
		break;
		case PULL_CONF:
			if((ret = pull_config_data()) !=0){
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


//declare the taskset with two task
static 	task_t taskset[2] = { 
	     {.id = 1,
	      .stime = 0,
	      .p=100,
	      .d=20,
	      .s=UNINIT,
	      .initf = &adc_init,
	      .loopf = &adc_pull_temp
	     },
	     {.id = 2,
	      .stime = 20,
	      .p=100,
	      .d=70,
	      .s=UNINIT,
	      .initf = &monitor_init,
	      .loopf = &monitor_temp_process
	     }
	};

int main(void){

	//Init HAL
	if(HAL_Init()) {
        zeiss_printf("hal_cannot be initialized \n");
        return -1;
    }

	if (init_shared_memory() != 0) {
         zeiss_printf("shared memory cannot be initialized \n");
        return -1;
    }

	//calculate the hyperperiod 
	uint32_t H = hyperperiod(taskset, 2);
	//Initialze ready queue 
	ready_queue_t * ready_queue;
	task_t *tset_ptr = taskset;
	initialize_queue(taskset,&ready_queue, 2,H);
	while(1) {
	spin(&tset_ptr, &ready_queue, 2, H);
	}
}
