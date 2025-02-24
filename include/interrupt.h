#ifndef __ZEISS_INTERRUPT_H__
#define __ZEISS_INTERRUPT_H__
#include <pthread.h>
#include <stdio.h>

typedef enum {IT_ENABLE=0x1, IT_DISABLE=!IT_ENABLE} ITStatus;

typedef enum {ADC1_DEV = 0x00, I2C1_DEV=0x00} InterruptID;

typedef struct {
	uint8_t t;

}InitInterruptStruct;

typedef struct {
	InterruptID it_id;
	void(*callback)(void);
	pthread_t thd_id;
	pthread_mutex_t mux;
	uint32_t interval; //Interrupt Interval
	ITStatus status;
}InterruptDef;



int IT_Init(InterruptDef ** it);
int IT_DInit(InterruptDef **it);


#endif //__ZEISS_INTERRUPT_H__
