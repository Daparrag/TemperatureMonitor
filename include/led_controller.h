#ifndef __ZEISS_LEDCONTROLLER_H__
#define __ZEISS_LEDCONTROLLER_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>


#define SHM_KEY 0x1010
#define SHM_SIZE 3  // 3 bytes for 3 LEDs

// Structure to hold shared memory
typedef struct {
    unsigned char led_states[SHM_SIZE];
} SharedMemory;


void signal_handler(int sig);
void cleanup(void);
int init_shared_memory(void);
void set_led(int led_idx, int state);
void set_all_leds(int state);
void toggle_let(int led_index);
void random_pattern(int duration);


#endif //__ZEISS_LED_CONTROLLER_H__

