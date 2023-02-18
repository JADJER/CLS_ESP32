cmake_minimum_required(VERSION 3.17)

set(SOURCES
        main.c
        bluetooth.c
        controller.c)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS .)
