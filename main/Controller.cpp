//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>

Controller::Controller() : m_indicator(2), m_button(0), m_pump(16) {
  log_i("Start bluetooth server...");
  m_indicator.blink(100);

  auto bluetooth = Bluetooth("HDS");

  m_bluetoothClient = bluetooth.createClient();

  m_bluetoothServer = bluetooth.createServer();
  m_bluetoothServer->createService(SERVICE_STATE_UUID, {STATE_CONNECTED_UUID});
  m_bluetoothServer->start();
  m_bluetoothServer->setValue(STATE_CONNECTED_UUID, "Connecting...");
  m_bluetoothServer->advertising();

  log_i("Connect to ECU...");
  m_indicator.blink(250);

  m_bluetoothServer->setValue(STATE_CONNECTED_UUID, "Connected");

  log_i("Detect active tables...");
  m_indicator.blink(500);

//  m_ecu.detectActiveTables();

  log_i("Initialize done");
  m_indicator.enable();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(10);
  }
}

void Controller::spinOnce() {
  if (m_button.isPressed()) {
    log_i("Detect all tables...");

    m_indicator.blink(500);
    m_ecu.detectAllTables();
    m_indicator.enable();

    m_button.resetState();
  }
}

void Controller::enablePump() {
}

void Controller::disablePump() {

}
