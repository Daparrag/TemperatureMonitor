#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include "led_controller.h"

volatile sig_atomic_t running = 1;
int shmid = -1;
SharedMemory* shm = NULL;

// Signal handler for clean shutdown
void signal_handler(int sig) {
    running = 0;
}

// Cleanup function
void cleanup() {
    if (shm != NULL) {
        // Detach shared memory
        if (shmdt(shm) == -1) {
            perror("shmdt");
        }
        shm = NULL;
    }
    //printf("\nLED Controller stopped\n");
}

// Initialize shared memory
int init_shared_memory() {
    // Try to get existing shared memory
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        printf("Error: LED simulation must be running first!\n");
        return -1;
    }

    // Attach shared memory
    shm = (SharedMemory*)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat");
        return -1;
    }

    return 0;
}

// Set specific LED state
void set_led(int led_index, int state) {
    if (led_index >= 0 && led_index < SHM_SIZE) {
        shm->led_states[led_index] = state ? 1 : 0;
    } else {
        printf("Error: Invalid LED index %d. Must be 0-2.\n", led_index);
    }
}

// Set all LEDs to the same state
void set_all_leds(int state) {
    for (int i = 0; i < SHM_SIZE; i++) {
        shm->led_states[i] = state ? 1 : 0;
    }
}

// Toggle specific LED state
void toggle_led(int led_index) {
    if (led_index >= 0 && led_index < SHM_SIZE) {
        shm->led_states[led_index] = shm->led_states[led_index] ? 0 : 1;
    } else {
        printf("Error: Invalid LED index %d. Must be 0-2.\n", led_index);
    }
}

// Generate random LED patterns
void random_pattern(int duration) {
    printf("Generating random patterns for %d seconds...\n", duration);
    
    time_t start_time = time(NULL);
    
    while (running && (time(NULL) - start_time) < duration) {
        for (int i = 0; i < SHM_SIZE; i++) {
            set_led(i, rand() % 2);
        }
        usleep(500000);  // Sleep for 0.5 seconds
    }
    
    set_all_leds(0);
    printf("All LEDs turned off\n");
}
/*
int main() {
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Initialize random seed
    srand(time(NULL));

    // Initialize shared memory
    if (init_shared_memory() != 0) {
        return 1;
    }

    printf("LED Controller started\n");
    printf("Available commands:\n");
    printf("1 - Toggle LED1\n");
    printf("2 - Toggle LED2\n");
    printf("3 - Toggle LED3\n");
    printf("r - Start random pattern\n");
    printf("on - Turn all LEDs on\n");
    printf("off - Turn all LEDs off\n");
    printf("q - Quit\n");

    char command[10];
    while (running) {
        printf("\nEnter command: ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            continue;
        }

        // Remove newline
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "q") == 0) {
            break;
        }
        else if (strcmp(command, "1") == 0) {
            toggle_led(0);
        }
        else if (strcmp(command, "2") == 0) {
            toggle_led(1);
        }
        else if (strcmp(command, "3") == 0) {
            toggle_led(2);
        }
        else if (strcmp(command, "r") == 0) {
            random_pattern(10);
        }
        else if (strcmp(command, "on") == 0) {
            set_all_leds(1);
        }
        else if (strcmp(command, "off") == 0) {
            set_all_leds(0);
        }
        else {
            printf("Invalid command!\n");
        }
    }

    cleanup();
    return 0;
}
*/
