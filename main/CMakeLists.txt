cmake_minimum_required(VERSION 3.17)

set(SOURCES
        main.cpp
        Pump.cpp
        Timer.cpp
        ExternalPower.cpp)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS .)
