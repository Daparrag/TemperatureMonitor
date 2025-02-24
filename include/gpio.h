#ifndef __ZEISS__GPIO_H__
#define __ZEISS__GPIO_H__

#include <stdint.h>
#include "device.h"

#define GPIO_1 (uint32_t) 0x0001
#define GPIO_2 (uint32_t) 0x0002
#define GPIO_3 (uint32_t) 0x0008
#define GPIO_4 (uint32_t) 0x0010
#define GPIO_5 (uint32_t) 0x0020
#define GPIO_6 (uint32_t) 0x0040
#define GPIO_7 (uint32_t) 0x0080
#define GPIO_8 (uint32_t) 0x0100
#define GPIO_9 (uint32_t) 0x0200
#define GPIO_10 (uint32_t)0x0400
#define IS_GPIO_PIN(GPIO_PIN) (((GPIO_PIN) == GPIO_1) || \
							   ((GPIO_PIN) == GPIO_2) || \
							   ((GPIO_PIN) == GPIO_3) || \
							   ((GPIO_PIN) == GPIO_4) || \
							   ((GPIO_PIN) == GPIO_5) || \
							   ((GPIO_PIN) == GPIO_6) || \
							   ((GPIO_PIN) == GPIO_7) || \
							   ((GPIO_PIN) == GPIO_8))
							   


#define GPIO_A (GPIO_2 | GPIO_3 | GPIO_4)
#define GPIO_B (GPIO_5 | GPIO_6 | GPIO_7)

typedef struct {
    __IO uint32_t MODER; /*GPIO Mode register*/
    __IO uint32_t OTYPER; /*GPIO port output type */
    __IO uint32_t IDR;  /*GPIO input data register*/
    __IO uint32_t ODR; /*GPIO Output data register*/
}GPIO_TypeDef; 


typedef enum{
	GPIO_Mode_IN = 0x00, 
        GPIO_Mode_OUT = 0x01, 
	GPIO_MODE_AN = 0x03,
	 GPIO_Mode_AF= 0x04, /*!< GPIO Alternate function Mode */
}GPIOModeTypeDef;

typedef enum{
	GPIO_PuPd_NOPULL = 0x00,
	GPIO_PuPd_DOWN = 0x01,
	GPIO_PuPd_UP = 0x02
}GPIOPuPd_TypeDef;

typedef struct{
	uint32_t GPIO_pin; /*Specified the pin to be configured*/
	GPIOModeTypeDef	GPIO_Mode; /*Specified the operating mode of the pin*/
	GPIOPuPd_TypeDef GPIO_PuPd; /*specifies the operating PullUp PullDown for the selected pin*/
}GPIO_InitTypeDef;

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStuct);
void GPIO_InitBus(GPIO_TypeDef * BUSx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t PortVal);

static GPIO_TypeDef GPIO1_dev;
static GPIO_TypeDef GPIO2_dev;
static GPIO_TypeDef GPIO3_dev;
static GPIO_TypeDef GPIO4_dev;
static GPIO_TypeDef GPIO5_dev;
static GPIO_TypeDef GPIO6_dev;
static GPIO_TypeDef GPIO7_dev;
static GPIO_TypeDef GPIO8_dev;
static GPIO_TypeDef GPIO9_dev;
static GPIO_TypeDef GPIO10_dev;
static GPIO_TypeDef GPIOC_dev;



#endif // __ZEISS_GPIO_H_ 
