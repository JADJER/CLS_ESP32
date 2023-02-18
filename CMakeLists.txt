cmake_minimum_required(VERSION 3.17)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(CLS
        VERSION 0.4.1
        DESCRIPTION "Chain lubrication system"
        HOMEPAGE_URL https://github.com/jadjer/CLS_ESP32
        LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
