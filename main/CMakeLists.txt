cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        BLE2901.cpp
        Distance.cpp
        Controller.cpp
        ServerCallback.cpp
        AdvertisedDevice.cpp
        SetDelayCallback.cpp
        SetDistanceCallback.cpp)

set(COMPONENTS
        arduino
        indicator
        button
        pump)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS . REQUIRES ${COMPONENTS})
