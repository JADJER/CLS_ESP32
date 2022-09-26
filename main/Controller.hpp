//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Bluetooth.hpp"
#include "Button.hpp"
#include "Indicator.hpp"
#include "Pump.hpp"
#include <esp_err.h>

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
  BluetoothClient* m_bluetoothClient;
  BluetoothServer* m_bluetoothServer;

 private:
  void spinOnce();
};
