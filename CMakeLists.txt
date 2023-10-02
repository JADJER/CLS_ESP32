cmake_minimum_required(VERSION 3.17)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(chain_lubrication_system
        VERSION 0.5.6
        DESCRIPTION "Chain lubrication system for motorcycles"
        HOMEPAGE_URL https://github.com/jadjer/chain-lubrication-system.git
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
