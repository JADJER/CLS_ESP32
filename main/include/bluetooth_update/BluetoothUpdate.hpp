//
// Created by jadjer on 9/26/24.
//

#pragma once

#include "NimBLECharacteristic.h"

class OtaCharacteristicCallback : public NimBLECharacteristicCallbacks {
public:
  ~OtaCharacteristicCallback() override = default;

public:
  void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
  void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
  void onStatus(NimBLECharacteristic *pCharacteristic, int code) override;
  void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override;
};

class BluetoothUpdate {
public:
  BluetoothUpdate();
};
