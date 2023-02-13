cmake_minimum_required(VERSION 3.17)

set(SOURCES
        external_sensor.c)

set(COMPONENTS
        driver)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
