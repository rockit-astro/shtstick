cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD seeed_xiao_rp2040 CACHE STRING "Board type")
include(pico_sdk_import.cmake)
project(shtstick C CXX ASM)

pico_sdk_init()

add_executable(shtstick main.c)

pico_set_program_name(shtstick "shtstick")
pico_set_program_version(shtstick "1.0")
pico_enable_stdio_uart(shtstick 0)
pico_enable_stdio_usb(shtstick 1)

target_link_libraries(shtstick pico_stdlib hardware_i2c)
target_include_directories(shtstick PRIVATE ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(shtstick)
