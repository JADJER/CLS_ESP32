//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Button.hpp"
#include "Distance.hpp"
#include "Indicator.hpp"
#include "Power.hpp"
#include "Pump.hpp"
#include "ServerCallback.hpp"
#include "SetDelayCallback.hpp"
#include "SetDistanceCallback.hpp"
#include <BLEAdvertisedDevice.h>
#include <BLECharacteristic.h>
#include <BLEClient.h>
#include <BLEServer.h>

class Controller {
 public:
  Controller();
  ~Controller();

 public:
    [[maybe_unused]] [[noreturn]] void spin();

 private:
  int m_speed;
  Button m_button;
  Power m_power;
  Pump m_pump;
  Distance m_distance;
  BLEServer* m_server;
  BLEClient* m_client;
  Indicator* m_indicator;
  ServerCallback m_serverCallback;
  SetDelayCallback * m_setDelayCallback;
  SetDistanceCallback * m_setDistanceCallback;
  BLECharacteristic* m_settingDistance;
  BLECharacteristic* m_settingDelay;
  BLECharacteristic* m_monitorState;
  BLECharacteristic* m_monitorDistance;
  BLERemoteCharacteristic* m_vehicleSpeed;

 private:
  void sleep();
  void connectToServer();
  void updateCharacteristics();
  void manualLubricate();
  void spinPump();
};
