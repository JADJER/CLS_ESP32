// Copyright 2022 Pavel Suprunov
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
// Created by jadjer on 28.09.22.
//

#include "SetDistanceCallback.hpp"

#include <Arduino.h>

SetDistanceCallback::SetDistanceCallback() : BLECharacteristicCallbacks() {
  m_preferences.begin("settings");
  m_defaultValue = 500;
  m_preferenceKey = "distance";

  if (not m_preferences.isKey(m_preferenceKey.c_str())) {
    m_preferences.putUInt(m_preferenceKey.c_str(), m_defaultValue);
  }
}

SetDistanceCallback::~SetDistanceCallback() {
  m_preferences.end();
}

float SetDistanceCallback::getValue() {
  uint32_t value = getValueFromPreference();
  return static_cast<float>(value);
}

void SetDistanceCallback::onRead(BLECharacteristic* pCharacteristic) {
  uint32_t value = getValueFromPreference();
  pCharacteristic->setValue(value);
  log_i("Read value from preference");
}

void SetDistanceCallback::onWrite(BLECharacteristic* pCharacteristic) {
  auto length = pCharacteristic->getLength();
  if (length > 4) { return; }

  auto data = pCharacteristic->getData();

  uint32_t value = 0;

  for (size_t i = 0; i < length; i++) {
    value |= data[i] << (8 * i);
  }

  m_preferences.putUInt(m_preferenceKey.c_str(), value);
  log_i("Write distance %d", value);
}

uint32_t SetDistanceCallback::getValueFromPreference() {
  uint32_t value = m_preferences.getUInt(m_preferenceKey.c_str(), m_defaultValue);
  return value;
}
