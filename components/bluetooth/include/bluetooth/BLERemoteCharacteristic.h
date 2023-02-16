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

#include "bluetooth/BLERemoteDescriptor.h"
#include "bluetooth/BLERemoteService.h"
#include "bluetooth/BLEUUID.h"
#include "bluetooth/RTOS.h"

#include <esp_gattc_api.h>
#include <functional>
#include <string>

class BLERemoteService;
class BLERemoteDescriptor;
typedef std::function<void(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)> notify_callback;
/**
 * @brief A model of a remote %BLE characteristic.
 */
class BLERemoteCharacteristic {
   public:
    ~BLERemoteCharacteristic();

    // Public member functions
    bool canBroadcast();
    bool canIndicate();
    bool canNotify();
    bool canRead();
    bool canWrite();
    bool canWriteNoResponse();
    BLERemoteDescriptor* getDescriptor(BLEUUID uuid);
    std::map<std::string, BLERemoteDescriptor*>* getDescriptors();
    BLERemoteService* getRemoteService();
    uint16_t getHandle();
    BLEUUID getUUID();
    std::string readValue();
    uint8_t readUInt8();
    uint16_t readUInt16();
    uint32_t readUInt32();
    float readFloat();
    void registerForNotify(notify_callback _callback, bool notifications = true, bool descriptorRequiresRegistration = true);
    void writeValue(uint8_t* data, size_t length, bool response = false);
    void writeValue(std::string newValue, bool response = false);
    void writeValue(uint8_t newValue, bool response = false);
    std::string toString();
    uint8_t* readRawData();
    void setAuth(esp_gatt_auth_req_t auth);

   private:
    BLERemoteCharacteristic(uint16_t handle, BLEUUID uuid, esp_gatt_char_prop_t charProp, BLERemoteService* pRemoteService);
    friend class BLEClient;
    friend class BLERemoteService;
    friend class BLERemoteDescriptor;

    // Private member functions
    void gattClientEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t* evtParam);

    void removeDescriptors();
    void retrieveDescriptors();

    // Private properties
    BLEUUID m_uuid;
    esp_gatt_char_prop_t m_charProp;
    esp_gatt_auth_req_t m_auth;
    uint16_t m_handle;
    BLERemoteService* m_pRemoteService;
    FreeRTOS::Semaphore m_semaphoreReadCharEvt = FreeRTOS::Semaphore("ReadCharEvt");
    FreeRTOS::Semaphore m_semaphoreRegForNotifyEvt = FreeRTOS::Semaphore("RegForNotifyEvt");
    FreeRTOS::Semaphore m_semaphoreWriteCharEvt = FreeRTOS::Semaphore("WriteCharEvt");
    std::string m_value;
    uint8_t* m_rawData;
    notify_callback m_notifyCallback;

    // We maintain a map of descriptors owned by this characteristic keyed by a string representation of the UUID.
    std::map<std::string, BLERemoteDescriptor*> m_descriptorMap;
};     // BLERemoteCharacteristic

#endif /* CONFIG_BLUEDROID_ENABLED */
