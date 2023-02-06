cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDARD 23)

set(SOURCES
        main.c)

set(COMPONENTS
#        indicator
#        button
#        pump
#        distance
        pump
        config
        controller
        external_power)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})
