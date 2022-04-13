cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

pico_sdk_init()

project(garden)


add_executable(garden
    main.c
    temphum16.c
    hydro.c
    ambient.c
    temphum16.h
    hydro.h
    ambient.h
)

target_link_libraries(garden
    pico_stdlib
    hardware_i2c
    hardware_adc
)

pico_enable_stdio_usb(garden 1)
pico_enable_stdio_uart(garden 0)

pico_add_extra_outputs(garden)
