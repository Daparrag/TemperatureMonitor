#ifndef __ZEISS_DEVICE_H__
#define __ZEISS_DEVICE_H__
#include <stdio.h>

//#define IS_FUNCTIONAL_STATE(STATE)()
typedef enum {DISABLE=0, ENABLE=!DISABLE} FunctionalState;
#define __IO volatile 
static void assert_failed(uint8_t* file, uint32_t line) {

	return;
}


#endif // __ZEISS_DEVICE_H__/
