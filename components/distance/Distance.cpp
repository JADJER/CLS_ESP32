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

#include "Distance.hpp"

#include <Arduino.h>

Distance::Distance() {
  m_speed = 0;
  m_distance = 0;
  m_lastUpdate = millis();
}

Distance::~Distance() = default;

void Distance::updateSpeed(uint8_t speed) {
  std::lock_guard<std::mutex> lock(m_mutex);

  unsigned long currentTime = millis();
  unsigned long differentTime = currentTime - m_lastUpdate;

  float diffTimeH = static_cast<float>(differentTime) / 3600000;
  float distance = static_cast<float>(speed) * diffTimeH;

  m_distance = m_distance + distance;
  m_lastUpdate = currentTime;
}

float Distance::getDistance() {
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_distance;
}
