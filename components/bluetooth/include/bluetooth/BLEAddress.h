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

#include <esp_gap_ble_api.h>// ESP32 BLE
#include <string>

/**
 * @brief A %BLE device address.
 *
 * Every %BLE device has a unique address which can be used to identify it and form connections.
 */
class BLEAddress {
   public:
    /**
     * @brief Create an address from the native ESP32 representation.
     * @param [in] address The native representation.
     */
    explicit BLEAddress(esp_bd_addr_t const& address);
    /**
     * @brief Create an address from a hex string
     *
     * A hex string is of the format:
     * ```
     * 00:00:00:00:00:00
     * ```
     * which is 17 characters in length.
     *
     * @param [in] stringAddress The hex representation of the address.
     */
    explicit BLEAddress(std::string const& stringAddress);

   public:
    /**
     * @brief Determine if this address equals another.
     * @param [in] otherAddress The other address to compare against.
     * @return True if the addresses are equal.
     */
    bool equals(BLEAddress otherAddress);
    bool operator==(const BLEAddress& otherAddress) const;
    bool operator!=(const BLEAddress& otherAddress) const;
    bool operator<(const BLEAddress& otherAddress) const;
    bool operator<=(const BLEAddress& otherAddress) const;
    bool operator>(const BLEAddress& otherAddress) const;
    bool operator>=(const BLEAddress& otherAddress) const;
    /**
     * @brief Return the native representation of the address.
     * @return The native representation of the address.
     */
    esp_bd_addr_t* getNative();
    /**
     * @brief Convert a BLE address to a string.
     *
     * A string representation of an address is in the format:
     *
     * ```
     * xx:xx:xx:xx:xx:xx
     * ```
     *
     * @return The string representation of the address.
     */
    std::string toString();

   private:
    esp_bd_addr_t m_address;
};

#endif /* CONFIG_BLUEDROID_ENABLED */
