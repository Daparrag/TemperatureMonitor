/*
 * @author: Diego Alejandro Parra Guzman
 * sched.h is an implementation of EDF scheduling algorithm. 
 * this schedule a list of task.
 * */

#ifndef __ZEISS_EDF_SCHED_H__
#define __ZEISS_EDF_SCHED_H__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum {UNINIT = 0x0, RUNNING=0x1} task_state;

typedef struct{
	int16_t id;  //task id
	int stime;    // first execution of the task. 
	uint16_t p;  //task period
	uint16_t d;  // task deadline
	task_state s;
	int (*initf)(void); // init function. 
	int (*loopf)(void); // periodic function. 
}task_t;

typedef struct {
	int16_t task_id; //task id
	int16_t priority;//priority of the task
	int16_t time_left;// remain time for execution
	uint16_t arrival_time; // arrivaltime on the queue
	clock_t stime; //starting time for computation
	clock_t etime; //ending time computation
}ready_queue_t; 
/*
 * @brief read the list of task to process. 
 * @param tasklist list of task to execute  
 * */
int read_taskset(task_t **tasklist);
/*
 * @brief calculate the hyperperiod for a list of task. 
 * @param task_t: list of task to be processed. 
 * @param N: number of task to 
 * */
uint32_t hyperperiod(task_t* taskset, int N);
/* @ brief register a init function to a task;
 * @ param task_id: task id.
 * @ param fun: pointer to init function.
 * */
int register_initf(int task_id, int(*fun)());

/* @ brief register a periodic function to a task;
 * @ param task_id: task id.
 * @ param fun: pointer to periodic function.
 * */
int register_loopf(int task_id, int(*fun)());

/* @ brief iniatialize a execution queue. 
 * @ param queue: task queue.
 * @ param ntask: number of task.
 * */
void initialize_queue( task_t *taskset, ready_queue_t** queue, int n_task, int hperiod);

/* @ brief register a init function to a task;
 * @ param task_id: task id.
 * @ param fun: pointer to periodic function.
 * */
int update_queue(task_t ** taskset, ready_queue_t ** queue, 
		 int queue_len, int t, int N);

/* @ brief register a init function to a task;
 * @ param task_id: task id.
 * @ param fun: pointer to periodic function.
 * */
void print_ready_queue(ready_queue_t * queue, int queue_len);


/* @ brief scheduler implementation;
 * @ param taskset pointer to a taskset.
 * @ param ready_queue pointer to ready queue 
 * @ param N number of task.  
 * @ param H: hyperperiod.
 * */
void spin(task_t**taskset, ready_queue_t** queue, int N, int H);

#endif //_ZEISS_EDF_SCHED_H__ 
