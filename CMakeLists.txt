cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(hello_world
    main.cpp
    driver.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(hello_world 
    pico_stdlib
    hardware_gpio
    hardware_pwm
    hardware_pio
    pico_tone
)

add_subdirectory(pico-tone build)



# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(hello_world)