/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/

#ifndef __ZEISS_ADC_H__
#define __ZEISS_ADC_H__

#include "device.h"

//ADC Interruption Definition
#define ADC_IT_EOC                                 ((uint16_t)0x0205)  
#define ADC_IT_AWD                                 ((uint16_t)0x0106)  
#define ADC_IT_JEOC                                ((uint16_t)0x0407)  
#define ADC_IT_OVR                                 ((uint16_t)0x201A)  

typedef enum {r6Bits = 0x06, 
	      r8Bits = 0x08, 
	      r10Bits = 0x0A, 
	      r12Bits = 0x0C
}ADC_Resolution;

typedef enum {LittleIndian = 0x00, 
	      BigIndian=!LittleIndian
}ADC_Endianes;
typedef struct {
	uint32_t Resolution;	/*!< Set ADC resolution > 
				   12bits, 10bits, 8bits, 6bits*/
	uint32_t DataAlign;	/*!< Set ADC convertion data alignment*/
}ADC_InitTypeDef;

//#define __IO volatile

typedef struct {
	__IO uint32_t SR;   /*!<ADC Status Register>*/
	__IO uint32_t CR;  /*!<ADC Control Register>*/ 
	__IO uint32_t SMPR; /*!<ADC Sample time Register>*/
	__IO uint32_t DR;    /*!<ADC convertion value;>*/ 
	
	
}ADC_TypeDef;

static ADC_TypeDef ADC1_dev;
//#define ADC1    (ADC_TypeDef *) ADC1_BASE

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* InitStruct);
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, 
			      uint8_t ADC_SampleTime);
uint16_t ADC_GetConvertionValue(ADC_TypeDef * ADCx);
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);

bool ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);

void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);


#endif //__ZEISS_ADC_H__
