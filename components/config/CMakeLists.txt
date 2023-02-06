cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDARD 23)

set(SOURCES
        config.c)

set(COMPONENTS
        driver)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
