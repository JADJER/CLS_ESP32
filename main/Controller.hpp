//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Button.hpp"
#include "Distance.hpp"
#include "Indicator.hpp"
#include "Pump.hpp"
#include "ServerCallback.hpp"
#include <BLEAdvertisedDevice.h>
#include <BLECharacteristic.h>
#include <BLEClient.h>
#include <BLEServer.h>

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  Indicator* m_indicator;
  Button m_button;
  Pump m_pump;
  Distance m_distance;
  BLEServer* m_server;
  BLEClient* m_client;
  ServerCallback m_serverCallback;
  BLECharacteristic* m_settingDistance;
  BLECharacteristic* m_settingDelay;
  BLECharacteristic* m_monitorDistance;
  BLECharacteristic* m_monitorOilLevel;
  BLERemoteCharacteristic* m_vehicleSpeed;
  BLERemoteCharacteristic* m_vehicleState;

 private:
  void connectToServer();
  void updateCharacteristics();
  void manualLubricate();
  void spinPump();
};
