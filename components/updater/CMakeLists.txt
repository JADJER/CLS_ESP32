cmake_minimum_required(VERSION 3.17)

set(SOURCES
        Updater.cpp)

set(COMPONENTS
        app_update)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
