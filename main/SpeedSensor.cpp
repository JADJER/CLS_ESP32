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
// Created by jadjer on 3/19/24.
//

#include "SpeedSensor.hpp"

#include "gpio/InputPin.hpp"

constexpr const uint32_t wheelLength = 100;
constexpr const uint8_t numberOfDistanceSensorPin = 14;

SpeedSensor::SpeedSensor() :
    m_distanceSensorPin(std::make_unique<gpio::InputPin>(numberOfDistanceSensorPin, true)){

}

uint32_t SpeedSensor::getSpeed() const {
  auto const interval = m_distanceSensorPin->getLastIntervalInMicroSeconds();
  auto const speed = interval * wheelLength;

  return speed;
}
