cmake_minimum_required(VERSION 3.17)

set(SOURCES
        Bluetooth.cpp
        gatts_srv.cpp)

set(COMPONENTS
        nvs_flash
        bt)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
