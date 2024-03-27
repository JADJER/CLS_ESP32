cmake_minimum_required(VERSION 3.17)

set(SOURCES
        main.cpp
        Controller.cpp
        Pump.cpp
        Timer.cpp
        SpeedSensor.cpp
        ExternalPower.cpp)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS .)
