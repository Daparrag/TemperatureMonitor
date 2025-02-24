/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/
#ifndef __ZEISS_HAL_DEVICE_H__
#define __ZEISS_HAL_DEVICE_H__
#include <stdio.h>
#include "device.h"

#define _I2C1 ((uint32_t)0x0001)    //I2C1 memory region
#define _I2C2 ((uint32_t)0x0002)    //I2C2 memory region
#define _I2C3 ((uint32_t)0x0003)    //I2C3 memory region
#define IS_I2C_DEVICE(I2C_DEVICE) (((I2C_DEVICE) == _I2C1) || \
                                   ((I2C_DEVICE) == _I2C2) || \
                                   ((I2C_DEVICE) == _I2C3))

#define _I2C_Mode_I2C           ((uint16_t)0x0000)
#define _I2C_Mode_SMBusDevice   ((uint16_t)0x0002)
#define _I2C_Mode_SMBusHost     ((uint16_t)0x000A)
#define IS_I2C_MODE(I2C_MODE)   (((I2C_MODE) == _I2C_Mode_I2C) || \
                                 ((I2C_MODE) == _I2C_Mode_SMBusDevice) || \
                                 ((I2C_MODE) == _I2C_Mode_SMBusHost))

//I2C DutyCicle 
#define _I2C_DutyCycle_16_9              ((uint16_t)0x4000) /*!< I2C fast mode Tlow/Thigh = 16/9 */
#define _I2C_DutyCycle_2                 ((uint16_t)0xBFFF) /*!< I2C fast mode Tlow/Thigh = 2 */
#define IS_I2C_DUTYCYCLE(I2C_DUTYCYCLE)  (((I2C_DUTYCYCLE) == _I2C_DutyCycle_16_9) || \
                                           ((I2C_DUTYCYCLE) == _I2C_DutyCycle_2)) 
//I2C ACK acknoledge 
#define _I2C_Ack_Enable                  ((uint16_t)0x0400)
#define _I2C_Ack_Disable                 ((uint16_t)0x0000)
#define IS_I2C_ACKFLAG(ACK_FLAG)         (((ACK_FLAG) == _I2C_Ack_Enable) || \
                                          ((ACK_FLAG) == _I2C_Ack_Disable))


// I2C TransferDirection
#define  _I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  _I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_TRANSFERDIRECTION(I2C_TF_DIR)  (((I2C_TF_DIR) == _I2C_Direction_Transmitter) || \
                                             ((I2C_TF_DIR) == _I2C_Direction_Receiver))

#define _ADC1  ((uint32_t)0x0010)   //ADC1 memory region
#define _ADC2  ((uint32_t)0x0011)   //ADC2 memory region
#define _ADC3  ((uint32_t)0x0012)   //ADC3 memory region
#define IS_ADC_DEVICE(ADC_DEVICE) (((ADC_DEVICE) == _ADC1) || \
                                   ((ADC_DEVICE) == _ADC2) || \
                                   ((ADC_DEVICE) == _ADC3))


#define _GPIO1  ((uint32_t)0x0020)  //GPIO1 memory region
#define _GPIO2  ((uint32_t)0x0021)  //GPIO2 memory region
#define _GPIO3  ((uint32_t)0x0022)  //GPIO3 memory region
#define _GPIO4  ((uint32_t)0x0023)  //GPIO4 memory region
#define _GPIO5  ((uint32_t)0x0024)  //GPIO5 memory region
#define _GPIO6  ((uint32_t)0x0025)  //GPIO6 memory region
#define _GPIO7  ((uint32_t)0x0026)  //GPIO7 memory region
#define _GPIO8  ((uint32_t)0x0027)  //GPIO8 memory region
#define IS_GPIO_DEVICE(GPIO_DEVICE) (((GPIO_DEVICE) == _GPIO1) ||  \
                                   ((GPIO_DEVICE) == _GPIO2)  ||  \
                                   ((GPIO_DEVICE) == _GPIO3)  ||  \
                                   ((GPIO_DEVICE) == _GPIO4)  ||  \
                                   ((GPIO_DEVICE) == _GPIO5)  ||  \
                                   ((GPIO_DEVICE) == _GPIO6)  ||  \
                                   ((GPIO_DEVICE) == _GPIO7)  ||  \
                                   ((GPIO_DEVICE) == _GPIO8))
//GPIO_BUS 
#define _GPIOA ((uint32_t)0x0080)
#define _GPIOB ((uint32_t)0x0080)
#define _GPIOC ((uint32_t)0x0080)
#define IS_GPIO_BUS(GPIO_BUS) (((GPIO_BUS) == _GPIOA) ||  \
                               ((GPIO_BUS) == _GPIOB) ||  \
                               ((GPIO_BUS) == _GPIOC))


#define _GPIO_MOD_IN ((uint8_t)0x00)  // GPIO Input Mode
#define _GPIO_MOD_OUT ((uint8_t)0x01) // GPIO Output Mode
#define _GPIO_MOD_AN ((uint8_t)0x02)  // GPIO Analog Mode
#define _GPIO_MOD_AF ((uint8_t)0x03)  // GPIO Aternative Function Mode
#define IS_GPIO_MODE(GPIO_MODE) (((GPIO_MODE) == _GPIO_MOD_IN) || \
                                ((GPIO_MODE) == _GPIO_MOD_OUT) || \
                                ((GPIO_MODE) == _GPIO_MOD_AN)  || \
                                ((GPIO_MODE) == _GPIO_MOD_AF))

#define _GPIO_PMOD_NOPULL ((uint8_t)0x00)  // GPIO not pull mode
#define _GPIO_PMOD_PULLDOWN ((uint8_t)0x01)  // GPIO pull down mode
#define _GPIO_PMOD_PULLUP ((uint8_t)0x02)  // GPIO pull down mode
#define IS_GPIO_PM_MODE(GPIO_PM_MODE) (((GPIO_PM_MODE) == _GPIO_PMOD_NOPULL)   || \
                                       ((GPIO_PM_MODE) == _GPIO_PMOD_PULLDOWN) || \
                                       ((GPIO_PM_MODE) == _GPIO_PMOD_PULLUP)




#endif // __ZEISS_DEVICE_H__/
