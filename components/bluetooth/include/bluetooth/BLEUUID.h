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

#if CONFIG_BLUEDROID_ENABLED

#include <esp_gatt_defs.h>
#include <string>

/**
 * @brief A model of a %BLE UUID.
 */
class BLEUUID {
   public:
    BLEUUID(std::string uuid);
    BLEUUID(uint16_t uuid);
    BLEUUID(uint32_t uuid);
    BLEUUID(esp_bt_uuid_t uuid);
    BLEUUID(uint8_t* pData, size_t size, bool msbFirst);
    BLEUUID(esp_gatt_id_t gattId);
    BLEUUID();
    uint8_t bitSize();// Get the number of bits in this uuid.
    bool equals(BLEUUID uuid);
    esp_bt_uuid_t* getNative();
    BLEUUID to128();
    std::string toString();
    static BLEUUID fromString(std::string uuid);// Create a BLEUUID from a string

   private:
    esp_bt_uuid_t m_uuid;   // The underlying UUID structure that this class wraps.
    bool m_valueSet = false;// Is there a value set for this instance.
};                          // BLEUUID

#endif                      /* CONFIG_BLUEDROID_ENABLED */
