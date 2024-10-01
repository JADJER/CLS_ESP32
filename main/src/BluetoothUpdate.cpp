//
// Created by jadjer on 9/26/24.
//

#include "bluetooth_update/BluetoothUpdate.hpp"

#include "NimBLEDevice.h"
#include "bluetooth/Identificator.hpp"

void OtaCharacteristicCallback::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
  NimBLECharacteristicCallbacks::onRead(pCharacteristic, connInfo);

  NimBLEUUID const uuid = pCharacteristic->getUUID();

  if (uuid == CHARACTERISTIC_PROGRESS_UUID) {
    printf("Read progress \n");
  }
}

void OtaCharacteristicCallback::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
  NimBLECharacteristicCallbacks::onWrite(pCharacteristic, connInfo);

  NimBLEUUID const uuid = pCharacteristic->getUUID();

  if (uuid == CHARACTERISTIC_RECEIVE_FIRMWARE_UUID) {
    auto const value = pCharacteristic->getValue<uint64_t>();

    printf("Write receive firmware %llu\n", value);
  }

  if (uuid == CHARACTERISTIC_COMMAND_UUID) {
    auto const value = pCharacteristic->getValue<uint64_t>();

    printf("Write command %llu\n", value);
  }

  if (uuid == CHARACTERISTIC_CUSTOMER_UUID) {
    auto const value = pCharacteristic->getValue<uint64_t>();

    printf("Write customer %llu\n", value);
  }
}

void OtaCharacteristicCallback::onStatus(NimBLECharacteristic *pCharacteristic, int code) {
  NimBLECharacteristicCallbacks::onStatus(pCharacteristic, code);

  printf("Status \n");
}

void OtaCharacteristicCallback::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {
  NimBLECharacteristicCallbacks::onSubscribe(pCharacteristic, connInfo, subValue);

  printf("Subscriber \n");
}

BluetoothUpdate::BluetoothUpdate() {
     NimBLEDevice::init("CLS");
}
