# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/../FreeRTOS-Kernel)
# Adjust path if different
include(FreeRTOS_Kernel_import.cmake)

project(countlock C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(countlock countlock.c )

pico_set_program_name(countlock "countlock")
pico_set_program_version(countlock "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(countlock 1)
pico_enable_stdio_usb(countlock 0)

# Add the standard library to the build
target_link_libraries(countlock
        pico_stdlib
        pico_multicore
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
    )

# Add the standard include files to the build
target_include_directories(countlock PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        include/
)

pico_add_extra_outputs(countlock)

