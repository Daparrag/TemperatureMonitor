/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/


#ifndef __ZEISS_HAL_H__
#define __ZEISS_HAL_H__
#include <stdint.h>
#include <stdbool.h>

#include "hal_device.h"
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "interrupt.h"
#include "time.h"

typedef void (*func_ptr)(void);

typedef struct 
{
    //define device drivers 
    ADC_TypeDef * ADC1;
    GPIO_TypeDef * GPIO1;
    GPIO_TypeDef * GPIO2;
    GPIO_TypeDef * GPIO3;
    GPIO_TypeDef * GPIO4;
    GPIO_TypeDef * GPIO5;
    GPIO_TypeDef * GPIO6;
    GPIO_TypeDef * GPIO7;
    GPIO_TypeDef * GPIO8;
    GPIO_TypeDef * GPIOC;
    I2C_TypeDef * I2C1;
    //Interruptions
    InterruptDef m_adc_it;
    InterruptDef m_i2c_it;
}Hal_Device;

ADC_TypeDef * Get_ADC_Device(uint32_t ADCx);
I2C_TypeDef * Get_I2C_Device(uint32_t I2Cx);
GPIO_TypeDef * Get_GPIO_Device(uint32_t GPIO);


//HAL Function
uint8_t  HAL_Init(void);

//GPIO Functions
uint8_t HAL_GPIO_Init(uint32_t GPIOx, int32_t GPIO_pin, uint8_t GPIO_Mode, uint8_t GPIO_PuPd_Mode);

int8_t HAL_GPIOBus_Init(uint32_t GPIOBusx, int32_t GPIO_pin, uint8_t GPIO_Mode, uint8_t GPIO_PuPd_Mode);


//I2C Functions
uint8_t  HAL_I2CInit(uint32_t I2Cx, uint32_t clockSpeed, uint8_t I2C_Mode, uint16_t I2C_DutyCycle, uint32_t I2C_addrs, uint32_t I2C_Ack);
uint8_t  HAL_I2CGenerateStart(uint32_t I2Cx, FunctionalState NewState);
uint8_t  HAL_I2C_Send7bitAddress(uint32_t I2Cx, uint8_t addrs, uint8_t I2C_Direction);
uint16_t  HAL_I2C_ReceiveData(uint32_t I2Cx);
uint8_t  HAL_I2C_GetItStatus(uint32_t I2Cx, uint32_t I2C_IT);
uint8_t  HAL_I2C_ClearITFlag(uint32_t I2Cx, uint32_t I2C_IT);
uint8_t HAL_I2C_GetReceivedData(uint32_t I2Cx, void** dest, size_t n_bites);

//ADC Functions 
uint8_t HAL_ADC_Init(uint32_t ADCx, uint32_t rsolution, uint32_t data_align);
uint8_t HAL_ADC_IT_Init(uint32_t ADCx, func_ptr fp, uint32_t interval);
uint16_t HAL_ADC_GetConvertionValue(uint32_t ADCx);
uint8_t HAL_ADC_ClearITPendingBit(uint32_t ADCx, uint32_t IT);
 








#endif //__ZEISS_HAL_H__