/*
 * @author: Diego Alejandro Parra Guzman 
 * sched.c: Implementation of EDF schedule 
 * */

#include <stdio.h>
#include <stdlib.h>
#include "edf_sched.h"



void initialize_queue(ready_task_t** queue, int n_task) {
	*queue = (ready_task_t*) malloc(sizeof(ready_task_t) * n_task);
	if (*queue != NULL){
	   for (int i=0; i < n_task; i++){
		(*queue)[i].task_id = -1; 
	   }
	}
};

int update_queue(task_t **taskset, ready_task_t** queue, 
		 int queue_len, int t, int N) 
{
	for(int i=0l; i < N; i++){
	  //Check if a process has to enter in the ready_queue. 
	  //
	  if ( ( (t - (*taskset)[i].stime) % (*taskset)[i].p == 0 )) 
	  {
		  queue_len++;
		  (*queue)[queue_len].task_id = (*taskset)[i].id;
		  (*queue)[queue_len].priority = -1 * (t + (*taskset)[i].d);
		  (*queue)[queue_len].time_left = (*taskset)[i].d;
		  (*queue)[queue_len].arrival_time = t;
		   
	  }
	}
	return queue_len;
}

void spin(task_t ** taskset, ready_task_t** queue, int n_task, int hperiod){
	int len;
	for (int t=0; t<hperiod; t++){
		
	}
}
