# Embedded Temperature Monitor

## Introduction

This project implements an embedded temperature monitor designed for microcontroller platforms. It leverages standard embedded peripherals like ADC (Analog-to-Digital Converter), GPIO (General Purpose Input/Output), and I2C (Inter-Integrated Circuit) to read temperature sensor data and potentially display the temperature or status using LEDs.

This project provides three distinct build targets, allowing for flexibility during development and deployment:

* **`baremetal`**: This target represents a more low-level or "bare metal" implementation. It likely interacts directly with the hardware registers for peripheral control, offering fine-grained control and potentially higher performance in resource-constrained environments. The entry point for this target is likely `src/main_br.c`.

* **`hal`**: This target utilizes a Hardware Abstraction Layer (HAL). A HAL provides a higher-level API to interact with the hardware, making the code more portable and easier to maintain. It abstracts away the direct hardware register manipulation, simplifying development and potentially improving code readability at the cost of some performance overhead. The entry point for this target is likely `src/main_hal.c`, and it includes `src/hal.c` which presumably contains the HAL implementation.

* **`sched`**: This target implementes the edf schedule for  enhance the system realtime capabilites. In contrast to the previous targets , where application tasks could execute freely without interruption, the EDF algorithm restricts execution by preempting tasks that exceed their deadline. This ensures that only tasks with earlier deadlines are executed, prioritizing timely completion over uninterrupted execution.

All targets share common modules for ADC, GPIO, I2C, Interrupt handling, and LED control, demonstrating a modular design that allows for code reuse and different levels of hardware abstraction.

## Compilation Instructions

This project uses CMake as its build system. To compile the project, you will need CMake installed on your system and a suitable toolchain for your system (Linux, Windows) you might need to configure CMake with one depending on your target.

Here are the steps to compile the project:

1.  **Create a Build Directory:** It's recommended to build CMake projects in a separate build directory to keep your source directory clean.

    ```bash
    mkdir build
    cd build
    ```

2.  **Run CMake to Configure the Project:**  From within the `build` directory, run CMake, pointing it to the root directory of your project (where the `CMakeLists.txt` file is located).

    ```bash
    cmake ..
    ```
    *   **Optional CMake Configuration:** If you need to specify a specific toolchain or build type (e.g., Debug, Release), you can add CMake arguments. For example, set the build type to Release:
        ```bash
            -DCMAKE_BUILD_TYPE=Release ..
        ```
        For basic compilation on a development machine, this might not be necessary. CMake often defaults to using your system's native compiler.

3.  **Build the Project:** After CMake configuration is successful, you can build the project using the following command:

    ```bash
    cmake --build .
    ```
    This command will build **all** targets defined in the `CMakeLists.txt` file, which in this case are `baremetal` and `hal`.

4.  **Building Specific Targets:** If you want to build only a specific target (either `baremetal` or `hal`), you can use the `--target` option:

    *   To build the `baremetal` target:
        ```bash
        cmake --build . --target baremetal
        ```

    *   To build the `hal` target:
        ```bash
        cmake --build . --target hal
        ```

5.  **Output Executables:** After a successful build, the executable files will be generated in the `build` directory (or a subdirectory within `build` depending on your generator and configuration). You will find:

    *   An executable named `baremetal` (or `baremetal.exe` on Windows) corresponding to the baremetal build.
    *   An executable named `hal` (or `hal.exe` on Windows) corresponding to the HAL build.

**CMake File Breakdown and Compilation Rules:**

The `CMakeLists.txt` file defines the compilation rules for this project. Let's break it down:

*   **`cmake_minimum_required(VERSION 3.5)`**:  Specifies that CMake version 3.5 or higher is required to process this file.
*   **`project(TemperatureMonitor VERSION 1.0.0 LANGUAGES C CXX)`**:  Declares the project name as "TemperatureMonitor", sets the project version to 1.0.0, and indicates that the project uses both C and C++ languages.  While C++ is mentioned, the source files listed seem to be only `.c` files, suggesting this project is primarily C-based.
*   **`set(BAREMETAL_SOURCE_FILES ...)`**: Defines a variable `BAREMETAL_SOURCE_FILES` that lists all the source files required to build the `baremetal` executable. These files are located in the `src` directory and include:
    *   `adc.c`
    *   `gpio.c`
    *   `i2c.c`
    *   `interrupt.c`
    *   `led_controller.c`
    *   `main_br.c`
*   **`set(HAL_SOURCE_FILES ...)`**: Defines a variable `HAL_SOURCE_FILES` that lists the source files for the `hal` executable. These files include all the files from `BAREMETAL_SOURCE_FILES` plus:
    *   `hal.c`
    *   `main_hal.c`
*   **`include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)`**:  Adds the `include` directory within the project's source directory to the include search path. This allows you to include header files from the `include` directory in your source code using `#include "header_file.h"`.
*   **`add_executable(baremetal ${BAREMETAL_SOURCE_FILES})`**:  This command defines the `baremetal` target as an executable. It tells CMake to compile all the source files listed in the `BAREMETAL_SOURCE_FILES` variable and link them together to create an executable named `baremetal`.
*   **`add_executable(hal ${HAL_SOURCE_FILES})`**: Similarly, this command defines the `hal` target as an executable. It compiles and links the source files listed in `HAL_SOURCE_FILES` to create an executable named `hal`.

## Execution instructions

In addition to the three applications, there is also a LED simulation implemented on python. you can refer to the file `led_simulator.py`. In order to test the programs. You should first launch the led_simulator script by using the following command
```bash
 python3 led_simulator.py
```

once the simulator start, the following interface should be displayed in the terminal:

```bash
LED Simulation (Press Ctrl+C to exit)

LED1   LED2   LED3
 ○      ○      ○
```
at this point, the user should start any applcation to see the results. For example to start the baremetal program the following command can be used 
```bash
./baremetal
```



**In summary, to compile this project, you need to use CMake to generate build files and then use CMake again to build the executables.  You have the option to build either the `baremetal` or `hal` target, or both, depending on your needs.**

---

**Author:** Diego Alejandro Parra Guzman
**Project:** Zeiss Interview-Temperature-Monitor
**Copyright:** All Rights Reserved