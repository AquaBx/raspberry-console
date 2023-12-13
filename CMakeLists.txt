cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(pico-game)

target_sources(pico-game PUBLIC
    main.cpp
    driver.cpp
    controller.cpp
    sound.cpp
)

target_link_libraries(pico-game 
    pico_stdlib
    hardware_gpio
    hardware_pwm
    hardware_pio
    pico_tone
)

add_subdirectory(pico-tone build)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pico-game)