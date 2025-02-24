#ifndef __ZEISS_I2C_H__
#define __ZEISS_I2C_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "device.h"

//Define Interrupt Flags
#define I2C_IT_BTF	((uint8_t)0x00)
#define I2C_IT_TIMEOUT	((uint8_t)0x01)

//I2C Mode
#define I2C_Mode_I2C                    ((uint16_t)0x0000)
#define I2C_Mode_SMBusDevice            ((uint16_t)0x0002)  
#define I2C_Mode_SMBusHost              ((uint16_t)0x000A)

//I2C DutyCicle 
#define I2C_DutyCycle_16_9              ((uint16_t)0x4000) /*!< I2C fast mode Tlow/Thigh = 16/9 */
#define I2C_DutyCycle_2                 ((uint16_t)0xBFFF) /*!< I2C fast mode Tlow/Thigh = 2 */
//I2C ACK acknoledge 
#define I2C_Ack_Enable                  ((uint16_t)0x0400)
#define I2C_Ack_Disable                 ((uint16_t)0x0000)
// I2C TransferDirection
#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)

typedef struct {
	__IO uint16_t CR;  /* I2C Control Register I2C Register*/
	__IO uint16_t OAR; /* I2C OWN Address Register*/
	__IO uint16_t DR;  /* I2C Data Address Register*/
	__IO uint16_t CCR; /* I2C Clock Control Register*/
	__IO uint16_t SR;  /* I2C Status Register */
}I2C_TypeDef;


typedef struct {
	uint32_t I2C_ClockSpeed; /*I2C Clock Frequency*/
	uint16_t I2C_Mode;	 /*Specifies I2C Mode*/
	uint16_t I2C_DutyCycle; /*I2C DutyCicle*/
	uint16_t I2C_addrs;	/*I2C Address*/
	uint16_t I2C_ack;	/*I2C ack: enable/disable acknoledge*/

}I2C_InitTypeDef;


void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_GenerateStart(I2C_TypeDef * I2Cx, FunctionalState NewState);
void I2C_GenerateStop(I2C_TypeDef * I2Cx, FunctionalState NewState);
void I2C_Send7bitAddress(I2C_TypeDef * I2Cx, uint8_t addrs, uint8_t I2C_Direction);
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
uint16_t I2C_ReceiveData(I2C_TypeDef * I2Cx);
void I2C_ClearITFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
bool I2C_GetItStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT);

static I2C_TypeDef I2C1_dev;

#endif //__ZEISS_I2C_H__
