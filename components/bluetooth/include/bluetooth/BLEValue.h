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

#include <string>

/**
 * @brief The model of a %BLE value.
 */
class BLEValue {
   public:
    BLEValue();
    void addPart(std::string part);
    void addPart(uint8_t* pData, size_t length);
    void cancel();
    void commit();
    uint8_t* getData();
    size_t getLength();
    uint16_t getReadOffset();
    std::string getValue();
    void setReadOffset(uint16_t readOffset);
    void setValue(std::string value);
    void setValue(uint8_t* pData, size_t length);

   private:
    std::string m_accumulation;
    uint16_t m_readOffset;
    std::string m_value;
};

#endif// CONFIG_BLUEDROID_ENABLED
