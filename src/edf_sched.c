/*
 * @author: Diego Alejandro Parra Guzman 
 * sched.c: Implementation of EDF schedule 
 * */
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "edf_sched.h"

static clock_t t1;
static clock_t t2;

/** @brief Calculate the hyperperiod of the task. 
 *  @param taskset: array of task to execute.
 *  @param N: taskset length
 */
uint32_t hyperperiod(task_t * taskset, int N){
	if (taskset == NULL) return -1;
	uint32_t lcm = taskset[0].p;
	uint32_t a,b;
	for (uint32_t i = 1; i < N; i++){
		a = lcm;
		b = taskset[i].p;
		while (a != b) {
			(a > b) ? (a = a - b) : (b = b - a);
/*
			if (a > b)
				a = a -b;
			else 
				b = b - a;
*/
		}
		lcm = (lcm * taskset[i].p) / a;
	}
	return lcm;
}


task_t* get_task_by_id(task_t*taskset, int n_task ,int task_id){
	for (int i=0; i<n_task; i++){
	  if (taskset[i].id == task_id);
		return &taskset[i];
	}
	return NULL;
}

void initialize_queue( task_t *taskset, ready_queue_t** queue, int n_task, int hperiod) {
	int l_hperiod = hperiod;
	int ltask = 0;
	if (taskset != NULL){
		for (int i =0; i < n_task; i ++){
			ltask +=1;
			l_hperiod -= taskset[i].d;
			if (l_hperiod == 0) break;
		}

		*queue = (ready_queue_t*) malloc(sizeof(ready_queue_t) * ltask);
		if (*queue != NULL){
	   		for (int i=0; i < ltask; i++){
				(*queue)[i].task_id = -1;
	   		}
		}
	}
};

int update_queue(task_t **taskset, ready_queue_t** queue, 
		 int queue_len, int t, int N) 
{
	for(int i=0; i < N; i++){
	  //Check if a process has to enter in the ready_queue. 
	  //
	  if ( ( (t - (*taskset)[i].stime) % (*taskset)[i].p) == 0 ) 
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

void spin(task_t ** taskset, ready_queue_t** ready_queue, int n_task, int hperiod){
	int len;
	int ready_length = -1;
	static int active_task = 0;
	static int prevous_task = -1;
	char message[256];
	static double ctime = 0;
	static double ptime = 0;
	for (int t=0; t<hperiod; t++){
		//Update the ready queue
	  ready_length = update_queue(taskset, ready_queue, ready_length, t, n_task);
	  int index = -1;
	  int task_id = -1;
	  int max_priority = -100;
	 
	  
	  //select suitable process from the ready queue. 
	  for( int i = 0 ; i <= ready_length ; i++){
		if ( (*ready_queue)[i].task_id != -1 ){
		   if ((*ready_queue)[i].priority > max_priority) {
			index = i;
			task_id = (*ready_queue)[i].task_id;
			max_priority = (*ready_queue)[i].priority;
			active_task = task_id;
		   }
		}
	  }
	  //lets run the selected task

	  if(task_id != -1){
		task_t * ttask= *taskset+index;
		if(ttask->s == UNINIT && ttask->initf != NULL) {
			ttask->initf();
			ttask->s = RUNNING;
		}else if(ttask->s == RUNNING && ttask->loopf != NULL){
			ttask->loopf();
		}

		//decrease the time left for the selected task. 
		(*ready_queue)[index].time_left -= ttask->d;
		if((*ready_queue)[index].time_left == 0){
			(*ready_queue)[index].task_id = -1;
			//t = t + ttask->d - 1;
			if (active_task != prevous_task && prevous_task !=-1){
				float t2 = ((ctime - ptime) / 10000.0);
				snprintf(message, sizeof(message), " [INFO] Task Id %d Took: %.4f sec", prevous_task, t2);
	  			zeiss_printf(message);
				ptime = ctime;
				//prevous_task = active_task;
			}
			prevous_task = active_task;
		}
		//print task with the schedule
	  }
	  ctime +=1;
	
	}
}
