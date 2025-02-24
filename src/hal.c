#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include "hal_device.h"
#include "hal.h"

static bool hal_init = false;
static Hal_Device zeiss_microcontroller = {.ADC1 = &ADC1_dev,
                                           .GPIO1 = &GPIO1_dev,
                                           .GPIO2 = &GPIO2_dev,
                                           .GPIO3 = &GPIO3_dev,
                                           .GPIO4 = &GPIO4_dev,
                                           .GPIO5 = &GPIO5_dev,
                                           .GPIO6 = &GPIO6_dev,
                                           .GPIO7 = &GPIO7_dev,
                                           .GPIO8 = &GPIO8_dev,
                                           .I2C1 = &I2C1_dev,
                                           .m_adc_it = {.callback = NULL, 
                                                         .interval = 1,
                                                         .mux = PTHREAD_MUTEX_INITIALIZER,
                                                         .status = IT_DISABLE
                                                       } 
                                          }; 




bool IS_HAL_INIT(){
    return hal_init;
}
/** @brief Initialize Hal 
 * @return: 0 if succeed or 1 otherwise.
 */
uint8_t HAL_Init(void){
    if (IS_HAL_INIT()) return 1;
    hal_init = true;
    return 0;

};


ADC_TypeDef * Get_ADC_Device(uint32_t ADCx){
    if  (!IS_ADC_DEVICE(ADCx) || 
         !IS_HAL_INIT()){
            return NULL;
         }

         if (ADCx == _ADC1){
            return zeiss_microcontroller.ADC1; 
         }
         /** TODO: Add support to other ADC devices*/
         return NULL; 
};


I2C_TypeDef * Get_I2C_Device(uint32_t I2Cx){
    if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return NULL;
         }

         if (I2Cx == _I2C1){
            return zeiss_microcontroller.I2C1;
         }
         /** TODO: Add support to other I2C devices*/
         return NULL; 
};

GPIO_TypeDef * Get_GPIO_Device(uint32_t GPIOx){
    if  (!IS_GPIO_DEVICE(GPIOx) || 
         !IS_HAL_INIT()){
            assert_failed(__FILE__, __LINE__);
         }

         if (GPIOx == _GPIO1){
            return zeiss_microcontroller.GPIO1; 
         }else if (GPIOx == _GPIO2){
            return zeiss_microcontroller.GPIO2; 
         }else if (GPIOx == _GPIO3){
            return zeiss_microcontroller.GPIO3; 
         }else if (GPIOx == _GPIO4){
            return zeiss_microcontroller.GPIO4; 
         }else if (GPIOx == _GPIO5){
            return zeiss_microcontroller.GPIO5; 
         }else if (GPIOx == _GPIO6){
            return zeiss_microcontroller.GPIO6; 
         }else if (GPIOx == _GPIO7){
            return zeiss_microcontroller.GPIO7; 
         }else if (GPIOx == _GPIO8){
            return zeiss_microcontroller.GPIO8; 
         }
         /** TODO: Add support to other ADC devices*/
         return NULL; 
};

//GPIO Functions
/** @brief Initialize the a GPIO perpheral
 * @param GPIOx: GPIO device 
 * @param GPIO_pin: Pin to use
 * @param GPIO_Mode: Pin Mode
 * @param GPIO_PuPd_Mode: configure Pin Pull up, pull down mode
 * @return 0 if succeed or 1 otherwise
 */
uint8_t HAL_GPIO_Init(uint32_t GPIOx, int32_t GPIO_pin, uint8_t GPIO_Mode, uint8_t GPIO_PuPd_Mode) {
        GPIO_InitTypeDef gpio_conf;
        GPIO_TypeDef *gpio_dev;

        if  (!IS_GPIO_DEVICE(GPIOx) || 
             !IS_GPIO_PIN(GPIO_pin) || 
             !IS_GPIO_MODE(GPIO_Mode)||
             !IS_GPIO_MODE(GPIO_PuPd_Mode) || 
             !IS_HAL_INIT()) {
                return 1;
            }

        gpio_dev = Get_GPIO_Device(GPIOx);
        if (gpio_dev == NULL) {
            return 1;
        }

        gpio_conf.GPIO_pin = GPIO_pin;
        gpio_conf.GPIO_Mode = GPIO_Mode;
        gpio_conf.GPIO_PuPd = GPIO_PuPd_Mode;
        GPIO_Init(gpio_dev, & gpio_conf);  
        return 0;   
};

int8_t HAL_GPIOBus_Init(uint32_t GPIOBusx, int32_t GPIO_pin, uint8_t GPIO_Mode, uint8_t GPIO_PuPd_Mode){

}


//I2C Functions

uint8_t  HAL_I2CInit(uint32_t I2Cx, uint32_t clockSpeed, 
                    uint8_t I2C_Mode, uint16_t I2C_DutyCycle, 
                    uint32_t I2C_addrs, uint32_t I2C_Ack) {

        I2C_InitTypeDef i2c_conf;
        I2C_TypeDef * i2c_dev; 
        if( !IS_I2C_DEVICE(I2Cx) || 
            !IS_I2C_MODE(I2C_Mode) ||
            !IS_I2C_DUTYCYCLE(I2C_DutyCycle) ||
            !IS_I2C_ACKFLAG(I2C_Ack)  ||
            !IS_HAL_INIT()
          ){
            return 1;
          }

          i2c_dev = Get_I2C_Device(I2Cx);
          if (i2c_dev == NULL) {
            return 1;
          }

          i2c_conf.I2C_ack = I2C_Ack;
          i2c_conf.I2C_addrs = I2C_addrs;
          i2c_conf.I2C_ClockSpeed = clockSpeed;
          i2c_conf.I2C_DutyCycle = I2C_DutyCycle;
          i2c_conf.I2C_Mode = I2C_Mode;

          I2C_Init(i2c_dev, &i2c_conf);
          return 0;



        
}

/** @brief Generates the I2C Start Signal
 * @param I2Cx: I2C device
 * @param NewState: Enable/Desable the Start Signal
 * @return 0 if succeed or 1 otherwise. 
 */
uint8_t HAL_I2CGenerateStart(uint32_t I2Cx, FunctionalState NewState) {
    I2C_TypeDef *i2c_dev;
    if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }

    i2c_dev = Get_I2C_Device(I2Cx);
    I2C_GenerateStart(i2c_dev, NewState);
    return 0;


};

/** @brief Transmits the address byte to select the slave device.
  * @param I2Cx: I2C device
  * @param addrs:specifies the slave address which will be transmitted
  * @param I2C_Direction: specifies whether the I2C device will be a Transmitter
  *         or a Receiver. 
  * @return 0 if succeed or 1 otherwise
  */
uint8_t  HAL_I2C_Send7bitAddress(uint32_t I2Cx, uint8_t addrs, uint8_t I2C_Direction) {
        I2C_TypeDef *i2c_dev;
        if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }

         i2c_dev = Get_I2C_Device(I2Cx);
         I2C_Send7bitAddress(i2c_dev, addrs, I2C_Direction);
         return 0;

};

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: the I2C peripheral.
  * @retval The value of the received data.
  */
uint16_t  HAL_I2C_ReceiveData(uint32_t I2Cx) {
        I2C_TypeDef *i2c_dev;
        if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }
         i2c_dev = Get_I2C_Device(I2Cx);
         return I2C_ReceiveData(i2c_dev);
         


};

/** @brief Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2Cx: the I2C peripheral.
  * @param  I2C_IT: specifies the interrupt source to check.
 * @return Flag Status
 */
uint8_t  HAL_I2C_GetItStatus(uint32_t I2Cx, uint32_t I2C_IT) {
     I2C_TypeDef *i2c_dev;
     if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }
    i2c_dev = Get_I2C_Device(I2Cx);
    return I2C_GetItStatus(i2c_dev, I2C_IT);
    

};

/**
  * @brief  Clears the I2Cx's pending flags.
  * @param  I2Cx: select the I2C peripheral.
  * @param  I2C_FLAG: specifies the flag to clear. 
  *          This parameter can be any combination of the following values:
  *            @arg I2C_FLAG_SMBALERT: SMBus Alert flag
  *            @arg I2C_FLAG_TIMEOUT: Timeout or Tlow error flag
  *            @arg I2C_FLAG_PECERR: PEC error in reception flag
  *            @arg I2C_FLAG_OVR: Overrun/Underrun flag (Slave mode)
  *            @arg I2C_FLAG_AF: Acknowledge failure flag
  *            @arg I2C_FLAG_ARLO: Arbitration lost flag (Master mode)
  *            @arg I2C_FLAG_BERR: Bus error flag
  *
  */
uint8_t  HAL_I2C_ClearITFlag(uint32_t I2Cx, uint32_t I2C_IT) {
    I2C_TypeDef *i2c_dev;
    if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }
    i2c_dev = Get_I2C_Device(I2Cx);
    if (i2c_dev == NULL) return 1;
    I2C_GetItStatus(i2c_dev, I2C_IT);

};

uint8_t HAL_I2C_GetReceivedData(uint32_t I2Cx, void** dest, size_t n_bites){
     I2C_TypeDef *i2c_dev;
    if  (!IS_I2C_DEVICE(I2Cx) || 
         !IS_HAL_INIT()){
            return 1;
         }
    i2c_dev = Get_I2C_Device(I2Cx);
    memcpy((void*)*dest, (const void*)&i2c_dev->DR, n_bites);
    return 0;
}

//ADC Functions 

/** @brief Initializes the ADCx peripheral according to parameters
 * @param ADCx: Select the ADC peripheral
 * @param resolution: ADC Resolution 
 * @param data_align: Big indian // Little Indian data align
 * @return 0 if succeed or 1 otherwise
 * */
uint8_t HAL_ADC_Init(uint32_t ADCx, uint32_t resolution, uint32_t data_align) {
        ADC_InitTypeDef adc_config;
        ADC_TypeDef * adc_dev;

        if(!IS_ADC_DEVICE(ADCx) ||
           !IS_HAL_INIT()) {
             return 1;
           }
        adc_dev = Get_ADC_Device(ADCx);
        if(adc_dev == NULL) return 1;
        adc_config.Resolution = resolution;
        adc_config.DataAlign = data_align;
        ADC_Init(adc_dev, &adc_config);

};

/** @brief Initializes the ADCx Interruption according to parameters 
 *	  
 * @param ADCx: Select the ADC peripheral
 * @param fp: Function callback calling once the interrupt occours
 * @param data_align: Interrupt interval
 * @return 0 if succeed or 1 otherwise
 * */
uint8_t HAL_ADC_IT_Init(uint32_t ADCx, func_ptr fp, uint32_t interval) {
          InterruptDef* adc_it_ptr = &zeiss_microcontroller.m_adc_it;
          if(!IS_ADC_DEVICE(ADCx) ||
           !IS_HAL_INIT()) {
             return 1;
           } 
          
           adc_it_ptr->callback = fp;
           adc_it_ptr->interval = interval;
           adc_it_ptr->status = IT_DISABLE;
           IT_Init(&adc_it_ptr);
           return 0;
};

/** @brief Return the last conversion results
 * @param ADCx: select the ADC periferal.
 * @retval the Data Convertion Value
 * */
uint16_t HAL_ADC_GetConvertionValue(uint32_t ADCx) {
    ADC_TypeDef * adc_dev;
     if(!IS_ADC_DEVICE(ADCx) ||
           !IS_HAL_INIT()) {
             assert_failed(__FILE__, __LINE__);
           } 
     adc_dev = Get_ADC_Device(ADCx);
     if (adc_dev == NULL) assert_failed(__FILE__, __LINE__);
     return ADC_GetConvertionValue(adc_dev);


};

/** @brief clean pending Interrupt flag
 * @param ADCx: Selected ADC peripheral.
 * @param ADC_IT: specify the interrup source to enable/disable 
 * */
uint8_t HAL_ADC_ClearITPendingBit(uint32_t ADCx, uint32_t IT){
        ADC_TypeDef * adc_dev;
        if(!IS_ADC_DEVICE(ADCx) ||
           !IS_HAL_INIT()) {
             assert_failed(__FILE__, __LINE__);
           } 
     adc_dev = Get_ADC_Device(ADCx);
     if (adc_dev == NULL) assert_failed(__FILE__, __LINE__);
     ADC_ClearITPendingBit(adc_dev, IT);
     return 0;
};
