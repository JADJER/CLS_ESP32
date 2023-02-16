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
// Created by jadjer on 17.02.23.
//

#include "bluetooth/BLEDescriptor.h"

#include "bluetooth/BLEService.h"
#include "bluetooth/GeneralUtils.h"

#include <esp_err.h>
#include <iomanip>
#include <sstream>

#define NULL_HANDLE (0xffff)

BLEDescriptor::BLEDescriptor(const char* uuid, uint16_t len) :
    BLEDescriptor(BLEUUID(uuid), len) {
}

BLEDescriptor::BLEDescriptor(BLEUUID uuid, uint16_t max_len) :
    m_value() {
    m_bleUUID = uuid;
    m_value.attr_len = 0;                          // Initial length is 0.
    m_value.attr_max_len = max_len;                // Maximum length of the data.
    m_handle = NULL_HANDLE;                        // Handle is initially unknown.
    m_pCharacteristic = nullptr;                   // No initial characteristic.
    m_pCallback = nullptr;                         // No initial callback.

    m_value.attr_value = (uint8_t*)malloc(max_len);// Allocate storage for the value.
}

BLEDescriptor::~BLEDescriptor() {
    free(m_value.attr_value);
}

void BLEDescriptor::executeCreate(BLECharacteristic* pCharacteristic) {
    log_v(">> executeCreate(): %s", toString().c_str());

    if (m_handle != NULL_HANDLE) {
        log_e("Descriptor already has a handle.");
        return;
    }

    m_pCharacteristic = pCharacteristic;

    esp_attr_control_t control;
    control.auto_rsp = ESP_GATT_AUTO_RSP;
    m_semaphoreCreateEvt.take("executeCreate");
    esp_err_t errRc = ::esp_ble_gatts_add_char_descr(
        pCharacteristic->getService()->getHandle(),
        getUUID().getNative(),
        (esp_gatt_perm_t)m_permissions,
        &m_value,
        &control);
    if (errRc != ESP_OK) {
        log_e("<< esp_ble_gatts_add_char_descr: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
        return;
    }

    m_semaphoreCreateEvt.wait("executeCreate");
    log_v("<< executeCreate");
}

uint16_t BLEDescriptor::getHandle() const {
    return m_handle;
}

size_t BLEDescriptor::getLength() const {
    return m_value.attr_len;
}

BLEUUID BLEDescriptor::getUUID() const {
    return m_bleUUID;
}

uint8_t* BLEDescriptor::getValue() const {
    return m_value.attr_value;
}

void BLEDescriptor::handleGATTServerEvent(
    esp_gatts_cb_event_t event,
    esp_gatt_if_t gatts_if,
    esp_ble_gatts_cb_param_t* param) {
    switch (event) {
        case ESP_GATTS_ADD_CHAR_DESCR_EVT: {
            if (m_pCharacteristic != nullptr and m_bleUUID.equals(BLEUUID(param->add_char_descr.descr_uuid)) and m_pCharacteristic->getService()->getHandle() == param->add_char_descr.service_handle and m_pCharacteristic == m_pCharacteristic->getService()->getLastCreatedCharacteristic()) {
                setHandle(param->add_char_descr.attr_handle);
                m_semaphoreCreateEvt.give();
            }
            break;
        }
        case ESP_GATTS_WRITE_EVT: {
            if (param->write.handle == m_handle) {
                setValue(param->write.value, param->write.len);

                if (m_pCallback != nullptr) {
                    m_pCallback->onWrite(this);
                }
            }
            break;
        }
        case ESP_GATTS_READ_EVT: {
            if (param->read.handle == m_handle) {
                if (m_pCallback != nullptr) {
                    m_pCallback->onRead(this);
                }
            }
            break;
        }

        default:
            break;
    }
}

void BLEDescriptor::setCallbacks(BLEDescriptorCallbacks* pCallback) {
    log_v(">> setCallbacks: 0x%x", (uint32_t)pCallback);
    m_pCallback = pCallback;
    log_v("<< setCallbacks");
}

void BLEDescriptor::setHandle(uint16_t handle) {
    log_v(">> setHandle(0x%.2x): Setting descriptor handle to be 0x%.2x", handle, handle);
    m_handle = handle;
    log_v("<< setHandle()");
}

void BLEDescriptor::setValue(uint8_t* data, size_t length) {
    if (length > ESP_GATT_MAX_ATTR_LEN) {
        log_e("Size %d too large, must be no bigger than %d", length, ESP_GATT_MAX_ATTR_LEN);
        return;
    }
    m_value.attr_len = length;
    memcpy(m_value.attr_value, data, length);
    if (m_handle != NULL_HANDLE) {
        esp_ble_gatts_set_attr_value(m_handle, length, (const uint8_t*)data);
        log_d("Set the value in the GATTS database using handle 0x%x", m_handle);
    }
}

void BLEDescriptor::setValue(std::string value) {
    setValue((uint8_t*)value.data(), value.length());
}

void BLEDescriptor::setAccessPermissions(esp_gatt_perm_t perm) {
    m_permissions = perm;
}

std::string BLEDescriptor::toString() {
    char hex[5];
    snprintf(hex, sizeof(hex), "%04x", m_handle);
    std::string res = "UUID: " + m_bleUUID.toString() + ", handle: 0x" + hex;
    return res;
}

BLEDescriptorCallbacks::~BLEDescriptorCallbacks() = default;

void BLEDescriptorCallbacks::onRead(BLEDescriptor* pDescriptor) {
    log_d("BLEDescriptorCallbacks", ">> onRead: default");
    log_d("BLEDescriptorCallbacks", "<< onRead");
}

void BLEDescriptorCallbacks::onWrite(BLEDescriptor* pDescriptor) {
    log_d("BLEDescriptorCallbacks", ">> onWrite: default");
    log_d("BLEDescriptorCallbacks", "<< onWrite");
}
