// Copyright 2023 Pavel Suprunov
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
// Created by jadjer on 16.02.23.
//

#pragma once

#include "sdkconfig.h"

#if defined(CONFIG_BLUEDROID_ENABLED)

#include "bluetooth/BLE2902.h"
#include "bluetooth/BLECharacteristic.h"
#include "bluetooth/BLEDescriptor.h"
#include "bluetooth/BLEService.h"
#include "bluetooth/HIDTypes.h"

#define GENERIC_HID         0x03C0
#define HID_KEYBOARD        0x03C1
#define HID_MOUSE           0x03C2
#define HID_JOYSTICK        0x03C3
#define HID_GAMEPAD         0x03C4
#define HID_TABLET          0x03C5
#define HID_CARD_READER     0x03C6
#define HID_DIGITAL_PEN     0x03C7
#define HID_BARCODE         0x03C8
#define HID_BRAILLE_DISPLAY 0x03C9

class BLEHIDDevice {
   public:
    BLEHIDDevice(BLEServer*);
    virtual ~BLEHIDDevice();

    void reportMap(uint8_t* map, uint16_t);
    void startServices();

    BLEService* deviceInfo();
    BLEService* hidService();
    BLEService* batteryService();

    BLECharacteristic* manufacturer();
    void manufacturer(std::string name);
    // BLECharacteristic* 	pnp();
    void pnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
    // BLECharacteristic*	hidInfo();
    void hidInfo(uint8_t country, uint8_t flags);
    // BLECharacteristic* 	batteryLevel();
    void setBatteryLevel(uint8_t level);

    // BLECharacteristic* 	reportMap();
    BLECharacteristic* hidControl();
    BLECharacteristic* inputReport(uint8_t reportID);
    BLECharacteristic* outputReport(uint8_t reportID);
    BLECharacteristic* featureReport(uint8_t reportID);
    BLECharacteristic* protocolMode();
    BLECharacteristic* bootInput();
    BLECharacteristic* bootOutput();

   private:
    BLEService* m_deviceInfoService;                // 0x180a
    BLEService* m_hidService;                       // 0x1812
    BLEService* m_batteryService = 0;               // 0x180f

    BLECharacteristic* m_manufacturerCharacteristic;// 0x2a29
    BLECharacteristic* m_pnpCharacteristic;         // 0x2a50
    BLECharacteristic* m_hidInfoCharacteristic;     // 0x2a4a
    BLECharacteristic* m_reportMapCharacteristic;   // 0x2a4b
    BLECharacteristic* m_hidControlCharacteristic;  // 0x2a4c
    BLECharacteristic* m_protocolModeCharacteristic;// 0x2a4e
    BLECharacteristic* m_batteryLevelCharacteristic;// 0x2a19
};

#endif// CONFIG_BLUEDROID_ENABLED
