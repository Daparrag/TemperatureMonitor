/*
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#
*/
#ifndef __ZEISS_COMMON_H__
#define __ZEISS_COMMON_H__ 

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>



// Define Constants for adc 
#define ADC_RESOLUTION 12 //12-bit ADC 
#define ADC_LEVELS ( 1 << ADC_BITS ) -1 //4095


#define EEPROM_ADDR ((uint8_t)0x70)

// Create a mutex to synchronize access to the console
static pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

// static message 
static char app_message[512];

static void zeiss_printf(const char* message){
 pthread_mutex_lock(&console_mutex);
  printf("%s\n", message);
  //flush the output buffer to ensure the message is printed inmediatelly
  fflush(stdout);
  // Unlock the mutex after printing the message
  pthread_mutex_unlock(&console_mutex);
}


static char* get_timestamp(){
    time_t rawtime;
    struct tm* timeinfo;
    char* timestamp_str = (char*)malloc(20 * sizeof(char)); // Allocate enough space

    if (timestamp_str == NULL) {
        perror("Failed to allocate memory for timestamp");
        return NULL;
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the timestamp: YYYY-MM-DD HH:MM:SS.mmm (milliseconds are tricky portably)
    // We'll just do seconds for simplicity here. You might need platform-specific
    // methods for higher resolution timestamps.
    strftime(timestamp_str, 20, "%Y-%m-%d %H:%M:%S", timeinfo);

    return timestamp_str;
}


/*Define enum constants*/

typedef enum {
	NORMAL = 0x00,
	WARNING = 0x01,
	CRITICAL = 0x02 
}TemperatureStatus;

typedef enum {
	INIT = 0x00,
	PULL_CONF = 0x01,
	SENSING = 0x02,
	REPORTING = 0x03,
	ON_ERROR = 0x04
}MonitorStatus;

typedef enum {
 SENSOR_A=0x0, SENSOR_B=!SENSOR_A
}TempSensor;

typedef struct {
   uint8_t Rev; //Sensor Revision. 
   uint8_t Serial; // SerialNumber.// 
}HardwareRevision;


typedef struct{
  MonitorStatus Status;
  TempSensor ActiveSensor;
}TempMonitor;

/*Functions for updating the monitor status*/
static int SetMonitorNextState(TempMonitor* monitor, MonitorStatus nextState){
	if (monitor == NULL) {
		return -1;
	}
	monitor->Status = nextState;  
	return 0;
};

static int SetMonitorActiveSensor(TempMonitor*monitor, TempSensor SelectedSensor) {
	if (monitor == NULL){
		return -1;
	}
	monitor->ActiveSensor = SelectedSensor;
	return 0;
}

//Functions to update sensor value 
static uint16_t adc_value; 
static void update_adc_value(uint16_t new_val){
	
  pthread_mutex_lock(&m_mutex);
	adc_value = new_val;	
  pthread_mutex_unlock(&m_mutex);
}

static uint16_t get_adc_value(){
	uint16_t ret;
  pthread_mutex_lock(&m_mutex);
	ret = adc_value;	
  pthread_mutex_unlock(&m_mutex);
  return ret;
}

#endif //__ZEISS_COMMON_H__
