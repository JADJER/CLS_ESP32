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

#include <esp_gap_ble_api.h>

class BLESecurity {
   public:
    BLESecurity();
    virtual ~BLESecurity();
    void setAuthenticationMode(esp_ble_auth_req_t auth_req);
    void setCapability(esp_ble_io_cap_t iocap);
    void setInitEncryptionKey(uint8_t init_key);
    void setRespEncryptionKey(uint8_t resp_key);
    void setKeySize(uint8_t key_size = 16);
    void setStaticPIN(uint32_t pin);
    static char* esp_key_type_to_str(esp_ble_key_type_t key_type);

   private:
    esp_ble_auth_req_t m_authReq;
    esp_ble_io_cap_t m_iocap;
    uint8_t m_initKey;
    uint8_t m_respKey;
    uint8_t m_keySize;

};// BLESecurity

/*
 * @brief Callbacks to handle GAP events related to authorization
 */
class BLESecurityCallbacks {
   public:
    virtual ~BLESecurityCallbacks() {};

    /**
     * @brief Its request from peer device to input authentication pin code displayed on peer device.
     * It requires that our device is capable to input 6-digits code by end user
     * @return Return 6-digits integer value from input device
     */
    virtual uint32_t onPassKeyRequest() = 0;

    /**
     * @brief Provide us 6-digits code to perform authentication.
     * It requires that our device is capable to display this code to end user
     * @param
     */
    virtual void onPassKeyNotify(uint32_t pass_key) = 0;

    /**
     * @brief Here we can make decision if we want to let negotiate authorization with peer device or not
     * return Return true if we accept this peer device request
     */

    virtual bool onSecurityRequest() = 0;
    /**
     * Provide us information when authentication process is completed
     */
    virtual void onAuthenticationComplete(esp_ble_auth_cmpl_t) = 0;

    virtual bool onConfirmPIN(uint32_t pin) = 0;
};    // BLESecurityCallbacks

#endif// CONFIG_BLUEDROID_ENABLED
