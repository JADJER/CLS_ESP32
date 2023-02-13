cmake_minimum_required(VERSION 3.17)

set(SOURCES
        Configuration.cpp)

set(COMPONENTS
        nvs_flash)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
