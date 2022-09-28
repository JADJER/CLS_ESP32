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

#include "SetDelayCallback.hpp"

#include <Arduino.h>

SetDelayCallback::SetDelayCallback() : BLECharacteristicCallbacks() {
  m_preferences.begin("settings");
  m_preferenceKey = "delay";

  if (not m_preferences.isKey(m_preferenceKey.c_str())) {
    m_preferences.putUChar(m_preferenceKey.c_str(), 10);
  }
}

SetDelayCallback::~SetDelayCallback() {
  m_preferences.end();
}

void SetDelayCallback::onRead(BLECharacteristic* pCharacteristic) {
  uint16_t value = m_preferences.getUChar(m_preferenceKey.c_str());
  pCharacteristic->setValue(value);
  log_i("Read value from preference");
}

void SetDelayCallback::onWrite(BLECharacteristic* pCharacteristic) {
  auto length = pCharacteristic->getLength();
  if (length != 1) { return; }

  auto data = pCharacteristic->getData();

  uint16_t value = 0;

  for (size_t i = 0; i < length; i++) {
    value |= data[i] << (8 * i);
  }

  m_preferences.putUChar(m_preferenceKey.c_str(), value);
  log_i("Write distance %d", value);
}
