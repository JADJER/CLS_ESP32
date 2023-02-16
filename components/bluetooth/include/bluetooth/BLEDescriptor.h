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

#include "bluetooth/BLECharacteristic.h"
#include "bluetooth/BLEUUID.h"
#include "bluetooth/RTOS.h"

#include <esp_gatts_api.h>
#include <string>

class BLEService;
class BLECharacteristic;
class BLEDescriptorCallbacks;

/**
 * @brief A model of a %BLE descriptor.
 */
class BLEDescriptor {
   public:
    /**
     * @brief BLEDescriptor constructor.
     */
    explicit BLEDescriptor(const char* uuid, uint16_t max_len = 100);
    /**
     * @brief BLEDescriptor constructor.
     */
    explicit BLEDescriptor(BLEUUID uuid, uint16_t max_len = 100);
    /**
     * @brief BLEDescriptor destructor.
     */
    virtual ~BLEDescriptor();

   public:
    /**
     * @brief Get the BLE handle for this descriptor.
     * @return The handle for this descriptor.
     */
    [[nodiscard]] uint16_t getHandle() const;// Get the handle of the descriptor.
    /**
     * @brief Get the length of the value of this descriptor.
     * @return The length (in bytes) of the value of this descriptor.
     */
    [[nodiscard]] size_t getLength() const;// Get the length of the value of the descriptor.
    /**
     * Get the UUID of the descriptor.
     * @return
     */
    [[nodiscard]] BLEUUID getUUID() const;// Get the UUID of the descriptor.
    /**
     * @brief Get the value of this descriptor.
     * @return A pointer to the value of this descriptor.
     */
    [[nodiscard]] uint8_t* getValue() const;// Get a pointer to the value of the descriptor.

   public:
    /**
     * @brief Handle GATT server events for the descripttor.
     * @param [in] event
     * @param [in] gatts_if
     * @param [in] param
     */
    void handleGATTServerEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t* param);

   public:
    void setAccessPermissions(esp_gatt_perm_t perm);
    /**
     * @brief Set the callback handlers for this descriptor.
     * @param [in] pCallbacks An instance of a callback structure used to define any callbacks for the descriptor.
     */
    void setCallbacks(BLEDescriptorCallbacks* pCallbacks);
    /**
     * @brief Set the value of the descriptor.
     * @param [in] data The data to set for the descriptor.
     * @param [in] length The length of the data in bytes.
     */
    void setValue(uint8_t* data, size_t size);
    /**
     * @brief Set the value of the descriptor.
     * @param [in] value The value of the descriptor in string form.
     */
    void setValue(std::string value);

   public:
    /**
     * @brief Return a string representation of the descriptor.
     * @return A string representation of the descriptor.
     */
    std::string toString();

   private:
    friend class BLEDescriptorMap;
    friend class BLECharacteristic;
    BLEUUID m_bleUUID;
    uint16_t m_handle;
    BLEDescriptorCallbacks* m_pCallback;
    BLECharacteristic* m_pCharacteristic;
    esp_gatt_perm_t m_permissions = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;
    FreeRTOS::Semaphore m_semaphoreCreateEvt = FreeRTOS::Semaphore("CreateEvt");
    esp_attr_value_t m_value;

    /**
     * @brief Execute the creation of the descriptor with the BLE runtime in ESP.
     * @param [in] pCharacteristic The characteristic to which to register this descriptor.
     */
    void executeCreate(BLECharacteristic* pCharacteristic);
    /**
     * @brief Set the handle of this descriptor.
     * Set the handle of this descriptor to be the supplied value.
     * @param [in] handle The handle to be associated with this descriptor.
     * @return N/A.
     */
    void setHandle(uint16_t handle);
};

/**
 * @brief Callbacks that can be associated with a %BLE descriptors to inform of events.
 *
 * When a server application creates a %BLE descriptor, we may wish to be informed when there is either
 * a read or write request to the descriptors value.  An application can register a
 * sub-classed instance of this class and will be notified when such an event happens.
 */
class BLEDescriptorCallbacks {
   public:
    virtual ~BLEDescriptorCallbacks();

   public:
    /**
     * @brief Callback function to support a read request.
     * @param [in] pDescriptor The descriptor that is the source of the event.
     */
    virtual void onRead(BLEDescriptor* pDescriptor);
    /**
     * @brief Callback function to support a write request.
     * @param [in] pDescriptor The descriptor that is the source of the event.
     */
    virtual void onWrite(BLEDescriptor* pDescriptor);
};

#endif /* CONFIG_BLUEDROID_ENABLED */
