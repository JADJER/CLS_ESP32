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

#include "bluetooth/BLEAddress.h"

#include <string>
#include <sstream>

BLEAddress::BLEAddress(esp_bd_addr_t const& address) :
    m_address() {
    std::copy(std::begin(address), std::end(address), std::begin(m_address));
}

BLEAddress::BLEAddress(std::string const& stringAddress) :
    m_address() {
    if (stringAddress.length() != 17) return;

    std::copy(std::begin(stringAddress), std::end(stringAddress), std::begin(m_address));
}

bool BLEAddress::equals(BLEAddress otherAddress) {
    return std::equal(std::begin(otherAddress.m_address), std::end(otherAddress.m_address), std::begin(m_address));
}

bool BLEAddress::operator==(const BLEAddress& otherAddress) const {
    return std::equal(std::begin(otherAddress.m_address), std::end(otherAddress.m_address), std::begin(m_address));
}

bool BLEAddress::operator!=(const BLEAddress& otherAddress) const {
    return not std::equal(std::begin(otherAddress.m_address), std::end(otherAddress.m_address), std::begin(m_address));
}

esp_bd_addr_t* BLEAddress::getNative() {
    return &m_address;
}

std::string BLEAddress::toString() {
    std::stringstream ss;
    ss << m_address[0] << ":";
    ss << m_address[1] << ":";
    ss << m_address[2] << ":";
    ss << m_address[3] << ":";
    ss << m_address[4] << ":";
    ss << m_address[5];

    return ss.str();
}
