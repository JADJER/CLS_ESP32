cmake_minimum_required(VERSION 3.17)

set(INCLUDE_DIRS
        include
)

set(SOURCES
        app_main.cpp

        src/Pump.cpp
        src/Timer.cpp
        src/Controller.cpp
        src/WheelSensor.cpp
        src/Configuration.cpp
        src/ExternalPower.cpp
        src/ControllerBase.cpp
        src/BluetoothUpdate.cpp

        src/bluetooth/Bluetooth.cpp
        src/bluetooth/ConfigurationCharacteristicCallback.cpp
)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS ${INCLUDE_DIRS})
