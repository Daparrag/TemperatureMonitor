#
# @Author: Diego Alejandro Parra Guzman
# @Project: Zeiss Interview-Temperature-Monitor
# @All Rights Reserved
#

cmake_minimum_required(VERSION 3.5)
project(TemperatureMonitor VERSION 1.0.0 LANGUAGES C CXX)

set(BAREMETAL_SOURCE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/adc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/i2c.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/interrupt.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/led_controller.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main_br.c"
	)

	set(HAL_SOURCE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/adc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/i2c.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/interrupt.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/led_controller.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/hal.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main_hal.c"
	)	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(baremetal ${BAREMETAL_SOURCE_FILES})
add_executable(hal ${HAL_SOURCE_FILES})



