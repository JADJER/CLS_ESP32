//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "BLE2901.hpp"
#include "BlinkIndicator.hpp"
#include "ClientCallback.hpp"
#include "ErrorCodeIndicator.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>

Controller::Controller() : m_indicator(new BlinkIndicator(2)), m_button(0), m_pump(16, 17) {
  BLEDevice::init("CLS");

  log_i("Bluetooth server start ...");
  m_indicator->blink(100);

  m_server = BLEDevice::createServer();

  {
    auto service = m_server->createService(serviceSettingsUUID, 9);

    m_settingDistance = service->createCharacteristic(settingDistanceUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    m_settingDistance->addDescriptor(new BLE2901("Distance"));

    m_settingDelay = service->createCharacteristic(settingDelayUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    m_settingDelay->addDescriptor(new BLE2901("Delay"));

    service->start();
  }

  {
    auto service = m_server->createService(serviceMonitorUUID, 6);

    m_monitorOilLevel = service->createCharacteristic(monitorOilLevelUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_monitorOilLevel->addDescriptor(new BLE2901("Oil Level"));
    m_monitorOilLevel->addDescriptor(new BLE2902());

    service->start();
  }

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(serviceSettingsUUID);
  advertising->start();

  log_i("HDS start scan...");
  m_indicator->blink(250);

  m_client = nullptr;

  BLEScan* BLEScan = BLEDevice::getScan();
  BLEScan->setAdvertisedDeviceCallbacks(&m_advertisedDevice);
  BLEScan->setInterval(1349);
  BLEScan->setWindow(449);
  BLEScan->setActiveScan(true);
  BLEScan->start(5, false);

  log_i("Initialize done");
  m_indicator->enable();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(10);
  }
}

void Controller::spinOnce() {
  if (m_pump.getState() == PumpState::ERROR) { return; }

  if (not m_client and m_advertisedDevice.getAdvertisedDevice()) {
    connectToServer();
  }

  if (m_button.isPressed()) {

    m_indicator->blink(500);
    m_pump.enable(2000);

    m_button.resetState();
  }

  m_pump.spinOnce();

  switch (m_pump.getState()) {
    case PumpState::DISABLE:
      m_indicator->enable();
      break;
    case PumpState::ENABLE:
      break;
    case PumpState::ERROR:
      delete m_indicator;
      m_indicator = new ErrorCodeIndicator(2);
      m_indicator->blink(1);
      break;
  }
}

bool Controller::connectToServer() {
  auto device = m_advertisedDevice.getAdvertisedDevice();
  if (device == nullptr) {
    return false;
  }

  log_i("Forming a connection to %s", device->getAddress().toString().c_str());

  m_client = BLEDevice::createClient();
  log_i(" - Created client");
  m_client->setClientCallbacks(new ClientCallback());
  m_client->connect(device);
  log_i(" - Connected to server");
  m_client->setMTU(517);

  BLERemoteService* serviceVehicle = m_client->getService(serviceVehicleUUID);
  if (serviceVehicle == nullptr) {
    log_w("Failed to find our service UUID: %s", serviceVehicleUUID.toString().c_str());
    m_client->disconnect();
    return false;
  }
  log_i(" - Found our service");

  auto speedCharacteristic = serviceVehicle->getCharacteristic(vehicleSpeedUUID);
  if (speedCharacteristic == nullptr) {
    log_w("Failed to find our characteristic UUID: %s", vehicleSpeedUUID.toString().c_str());
    m_client->disconnect();
    return false;
  }
  log_i(" - Found our characteristic");

  if(speedCharacteristic->canRead()) {
    int speedValue = speedCharacteristic->readUInt8();
    log_i("The characteristic value was: %d", speedValue);
  }

  if(speedCharacteristic->canNotify()) {
//    speed->registerForNotify(notifyCallback);
  }

  return true;
}
