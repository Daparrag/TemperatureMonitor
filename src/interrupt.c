/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> 
#include "interrupt.h"
#include "common.h"
#include <time.h>
static InterruptDef *InterruptList[10];

void *thread_function(void * arg){
	InterruptDef *it = (InterruptDef*) arg;
	char itname[10] = "ADC IT"; 
	char message[256];
	if(it == NULL) {
	   return; 
	}
	uint32_t lid = (uint32_t)it->it_id;
	
	while(true) {
		pthread_mutex_lock(&it->mux);    // Lock the mutex
		ITStatus lstatus = it->status;  // GetControlValue
		pthread_mutex_unlock(&it->mux); // Unlock the mutex
		if (lstatus == IT_ENABLE) {
			clock_t start_time = clock();
			double ctime = 0;
			uint32_t tick = it->interval;
			while(tick){ tick --; ctime ++;}
			//calling the callback function 
			if (it->callback != NULL){
				it->callback();
			}

			clock_t end_time = clock();
			double time_passed = (double) 100 * (end_time - start_time) / CLOCKS_PER_SEC; 

			snprintf(message, sizeof(message), "[INFO] \t The Interrupt %s takes %.4f s",
			       itname, (ctime / 10000.0) );
			zeiss_printf(message);
			

		}else{
		   break;
		}
	}
	return NULL;
}

int IT_DInit(InterruptDef **it){
	if (it != NULL && *it !=NULL){
		InterruptDef * m_it = *it;
	}

	return 0;
}

int IT_Init(InterruptDef **it){

	if (it == NULL || *it == NULL) goto pinval;
	InterruptDef * m_it = *it;
	pthread_mutex_lock(&m_it->mux); 
	uint32_t lid = (uint32_t)m_it->it_id;
	ITStatus lstatus = m_it->status;
	if (lstatus == IT_ENABLE) goto sinval;
	//associate the interrupt struct to interrupt list	
	InterruptList[lid] = *it;
	//enable the interrupt;
	m_it->status = IT_ENABLE;
	//	
	pthread_create(&m_it->thd_id, NULL, thread_function, (void*)InterruptList[lid]);

sinval: pthread_mutex_unlock(&m_it->mux);
retval: return 0; 	
pinval: return -1; 
};
