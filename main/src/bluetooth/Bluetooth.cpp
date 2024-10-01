// Copyright 2024 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 9/25/24.
//

#include "bluetooth/Bluetooth.hpp"

#include "NimBLEDevice.h"

#include "bluetooth/Identificator.hpp"
#include "bluetooth/characteristic/ConfigurationCharacteristicCallback.hpp"

Bluetooth::Bluetooth(ConfigurationPtr const &configuration) : m_configurationCharacteristicCallback(std::make_unique<ConfigurationCharacteristicCallback>(configuration)) {

  NimBLEDevice::init("CLS");

  auto const server = NimBLEDevice::createServer();
  server->advertiseOnDisconnect(true);

  {
    auto const service = server->createService(SERVICE_CONFIGURATION_UUID);

    auto const pumpTimeoutCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_PUMP_TIMEOUT_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    pumpTimeoutCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    auto const minimalSpeedCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_MINIMAL_SPEED_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    minimalSpeedCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    auto const distanceForEnableCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_DISTANCE_FOR_ENABLE_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    distanceForEnableCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    auto const totalDistanceCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_TOTAL_DISTANCE_UUID,
        NIMBLE_PROPERTY::READ);
    totalDistanceCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    auto const nextDistanceCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_NEXT_DISTANCE_UUID,
        NIMBLE_PROPERTY::READ);
    nextDistanceCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    service->start();
  }

  {
    auto const service = server->createService(SERVICE_CONTROL_UUID);

    auto const manualLubricateCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_MANUAL_LUBRICATE_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    manualLubricateCharacteristic->setCallbacks(m_configurationCharacteristicCallback.get());

    service->start();
  }

  {
    auto const service = server->createService(SERVICE_OTA_UUID);

    auto const receiveFirmwareCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_RECEIVE_FIRMWARE_UUID,
        NIMBLE_PROPERTY::INDICATE | NIMBLE_PROPERTY::WRITE);
    receiveFirmwareCharacteristic->setCallbacks(m_otaCharacteristicCallback.get());

    auto const otaBarCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_PROGRESS_UUID,
        NIMBLE_PROPERTY::INDICATE | NIMBLE_PROPERTY::READ);
    otaBarCharacteristic->setCallbacks(m_otaCharacteristicCallback.get());

    auto const commandCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_COMMAND_UUID,
        NIMBLE_PROPERTY::INDICATE | NIMBLE_PROPERTY::WRITE);
    commandCharacteristic->setCallbacks(m_otaCharacteristicCallback.get());

    auto const customerCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_CUSTOMER_UUID,
        NIMBLE_PROPERTY::INDICATE | NIMBLE_PROPERTY::WRITE);
    customerCharacteristic->setCallbacks(m_otaCharacteristicCallback.get());

    service->start();
  }

  server->start();
}

Bluetooth::~Bluetooth() {
  NimBLEDevice::deinit();
}

void Bluetooth::advertise() {
  auto const advertising = NimBLEDevice::getAdvertising();
  //  advertising->addServiceUUID(SERVICE_CONFIGURATION_UUID);
  //  advertising->addServiceUUID(SERVICE_CONTROL_UUID);
  advertising->addServiceUUID(SERVICE_OTA_UUID);
  advertising->setManufacturerData("Pavel Suprunov");

  advertising->setScanResponse(true);
  advertising->start();
}
