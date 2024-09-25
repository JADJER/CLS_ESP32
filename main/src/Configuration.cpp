// Copyright 2024 Pavel Suprunov
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
// Created by jadjer on 9/20/24.
//

#include "configuration/Configuration.hpp"

constexpr uint64_t const HOUR = 60;
constexpr uint64_t const MINUTE = 60;
constexpr uint64_t const KILOMETER = 1000;

uint8_t Configuration::getExternalPowerPin() const {
  return 15;
}

uint8_t Configuration::getPumpPin() const {
  return 16;
}

uint8_t Configuration::getWheelSensorPin() const {
  return 14;
}

uint64_t Configuration::getPumpTimeout() const {
  return 60;
}

uint64_t Configuration::getWheelLength() const {
  return 1300;
}

float Configuration::getMinimalSpeed() const {
  return 5;
}

float Configuration::getDistanceForEnable() const {
  return 500 * KILOMETER;
}

float Configuration::getTotalDistance() const {
  return 0;
}

float Configuration::getNextDistance() const {
  return 0;
}

void Configuration::saveTotalDistance(float distance) {
}

void Configuration::saveNextDistance(float distance) {
}
