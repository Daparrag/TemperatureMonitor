#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <adc.h>


/** @brief Initializes the ADCx peripheral according to parameters
 *	  defined by the struct ADC_InitStruct.
 * @param ADCx: Select the ADC peripheral
 * @param ADC_InitStruct: Structure that contains the configuration
 *			  information for specific ADC peripheral. 
 * */

void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* InitStruct){
	//1. Check imput parameters
	if (ADCx == NULL || InitStruct == NULL ){
		assert_failed(__FILE__,__LINE__);
	}
	//
	uint32_t tmpreg = ADCx->CR;
	// write data to the register
	tmpreg |= InitStruct->Resolution | InitStruct->DataAlign;	
}

/** @brief Configures the Sampling Time for a selected ADCx Channel
 * @param ADCx: the selected ADC Peripheral.
 * @param ADC_Channel: The ADC_Channel to Configure.
 * @param ADC_SampleTime: The sample time value to be set for the selected
 * channel.
 * */
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, 
			      uint8_t ADC_SampleTime){
	if(ADCx == NULL) {
		assert_failed(__FILE__,__LINE__);
	}
	// Write Configuration
	uint32_t tmpreg = ADCx->SMPR;
	tmpreg |= (uint32_t)ADC_SampleTime << ADC_Channel;
	ADCx->SMPR = tmpreg;
}

/** @brief Return the last conversion results
 * @param ADCx: select the ADC periferal.
 * @retval the Data Convertion Value
 * */
uint16_t ADC_GetConvertionValue(ADC_TypeDef * ADCx){
	//Ligically we should ge that from a register. 
	if (ADCx == NULL){
		assert_failed(__FILE__,__LINE__);
	}

	ADCx->DR = rand() % 1100;
	return (uint16_t) ADCx->DR;
}

/** @brief Used to Enable or Desable The interrupt Source
 * @param ADCx: Selected ADC peripheral. 
 * @param ADC_IT: specify the interrupt source to enable/desable
 * @param NewState: New state of the specified ADC interrupts. 
 * */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState) {
	
	if (ADCx == NULL){
	   assert_failed(__FILE__, __LINE__);
	}
	// 
	uint32_t itmask = 0;
	itmask = (uint8_t)ADC_IT;
	if (NewState != DISABLE) {
		//Enable the selected ADC interrupts
		ADCx->CR |=itmask; 
	}else{
		//Disable the selected ADC interrupts.
		ADCx->CR &= ~((uint32_t)itmask);
	}
}


/** @brief Check whether the ADCx interrupt has occurred or not. 
 * */
bool ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT) {
	return true;
}

/** @brief clean pending Interrupt flag
 * @param ADCx: Selected ADC peripheral.
 * @param ADC_IT: specify the interrup source to enable/disable 
 * */

void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT){
	// Clean Pending Interupt Flag
	if (ADCx == NULL) {
	   assert_failed(__FILE__,__LINE__);
	}
	uint32_t itmask = (uint8_t)(ADC_IT >> 8);

	ADCx->SR = ~(uint32_t)itmask;
}

