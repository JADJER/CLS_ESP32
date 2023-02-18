cmake_minimum_required(VERSION 3.17)

set(SOURCES
        timer.c)

set(COMPONENTS)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
