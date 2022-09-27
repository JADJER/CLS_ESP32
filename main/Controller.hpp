//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "AdvertisedDevice.hpp"
#include "Button.hpp"
#include "Indicator.hpp"
#include "Pump.hpp"
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
  BLEServer* m_server;
  BLEClient* m_client;
  AdvertisedDevice m_advertisedDevice;
  BLECharacteristic* m_settingDistance;
  BLECharacteristic* m_settingDelay;
  BLECharacteristic* m_monitorOilLevel;

 private:
  void spinOnce();
  bool connectToServer();
};
