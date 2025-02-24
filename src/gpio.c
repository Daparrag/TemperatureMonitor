#include "gpio.h"

/*
 * @brief Initializes the GPIOx peripheral
 * @param GPIOx: The selected GPIO Device. 
 * @param GPIO_InitStruct: Structure that contains informantion for specified
 * GPIO Periferals
 * */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStuct){
	
}
/*
 * @brief sets the select data port bites
 * @param GPIOx: Selected GPIO Peripheral 
 * @param GPIO_Pin: combination of GPIO pins. 
 * */
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){

}

/*
 * @brief Sets or clears the selected data port bits 
 * @param GPIOx: Selected GPIO Peripheral 
 * @param GPIO_Pin: Specifies the port bit to be writen. 
 * @param BitVal: specifies the value to be written to the selected bit
 * */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t PortVal){

}
