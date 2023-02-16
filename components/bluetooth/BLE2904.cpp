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

#include "bluetooth/BLE2904.h"

BLE2904::BLE2904() :
    BLEDescriptor(BLEUUID((uint16_t)0x2904)),
    m_data() {
    m_data.m_format = 0;
    m_data.m_exponent = 0;
    m_data.m_namespace = 1;// 1 = Bluetooth SIG Assigned Numbers
    m_data.m_unit = 0;
    m_data.m_description = 0;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}// BLE2902

/**
 * @brief Set the description.
 */
void BLE2904::setDescription(uint16_t description) {
    m_data.m_description = description;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}

/**
 * @brief Set the exponent.
 */
void BLE2904::setExponent(int8_t exponent) {
    m_data.m_exponent = exponent;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}// setExponent

/**
 * @brief Set the format.
 */
void BLE2904::setFormat(uint8_t format) {
    m_data.m_format = format;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}// setFormat

/**
 * @brief Set the namespace.
 */
void BLE2904::setNamespace(uint8_t namespace_value) {
    m_data.m_namespace = namespace_value;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}// setNamespace

/**
 * @brief Set the units for this value.  It should be one of the encoded values defined here:
 * https://www.bluetooth.com/specifications/assigned-numbers/units
 * @param [in] unit The type of units of this characteristic as defined by assigned numbers.
 */
void BLE2904::setUnit(uint16_t unit) {
    m_data.m_unit = unit;
    setValue((uint8_t*)&m_data, sizeof(m_data));
}// setUnit
