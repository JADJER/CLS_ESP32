cmake_minimum_required(VERSION 3.17)

set(SOURCES
        Controller.cpp)

set(COMPONENTS
        pump
        configuration
        power_manager)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
