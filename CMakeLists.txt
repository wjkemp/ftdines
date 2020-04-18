project(ftdines)
cmake_minimum_required(VERSION 2.8)
add_executable(ftdines
    source/main.c
    source/uinput.c
    source/uinput.h
    source/keymap.c
    source/keymap.h
    source/input_channel.c
    source/input_channel.h
    source/ftdi_if.c
    source/ftdi_if.h
    source/debug.h
    )
target_link_libraries(ftdines ftdi config)
