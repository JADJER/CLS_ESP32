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
// Created by jadjer on 3/18/24.
//

#include "controller/Controller.hpp"

#include <esp_log.h>
#include <esp_sleep.h>

constexpr uint64_t const PER_MICROSECOND = 1000000;

Controller::Controller(ConfigurationPtr configuration) : m_configuration(std::move(configuration)),
                                                         m_pumpPtr(std::make_unique<Pump>(m_configuration->getPumpPin())),
                                                         m_timerPtr(std::make_unique<Timer>()),
                                                         m_wheelSensorPtr(std::make_unique<WheelSensor>(m_configuration->getWheelSensorPin(), m_configuration->getWheelLength())),
                                                         m_externalPowerPtr(std::make_unique<ExternalPower>(m_configuration->getExternalPowerPin())) {

  float const nextDistance = m_configuration->getNextDistance();
  float const totalDistance = m_configuration->getTotalDistance();

  if (nextDistance == 0 or nextDistance <= totalDistance) {
    float const distanceForEnable = m_configuration->getDistanceForEnable();

    m_configuration->saveNextDistance(totalDistance + distanceForEnable);
  }
}

void Controller::spinOnce() {
  if (not m_externalPowerPtr->isEnabled()) {
    sleep();
  }

  bool const lubricate = m_configuration->isLubricate();
  float const speed = m_wheelSensorPtr->getSpeed();
  float const actualDistance = m_wheelSensorPtr->getDistance();
  float const savedDistance = m_configuration->getTotalDistance();
  float const totalDistance = savedDistance + actualDistance;
  float const nextDistance = m_configuration->getNextDistance();

  ESP_LOGI("Controller",
           "Total distance: %f[m], next distance: %f[m], speed: %f[m/s], lubricate: %s, pump: %s",
           totalDistance,
           nextDistance,
           speed,
           lubricate ? "True" : "False",
           m_pumpPtr->isEnabled() ? "True" : "False");

  if (totalDistance >= nextDistance) {
    m_configuration->setLubricate(true);
  }

  float const minimalSpeed = m_configuration->getMinimalSpeed();

  if (speed < minimalSpeed) {
    pumpDisable();
    return;
  }

  if (lubricate) {
    pumpEnable();
  }
}

void Controller::sleep() {
  ESP_LOGI("Controller", "External power is disabled. Sleeping");

  float const actualDistance = m_wheelSensorPtr->getDistance();
  float const savedDistance = m_configuration->getTotalDistance();
  float const totalDistance = savedDistance + actualDistance;

  m_configuration->saveTotalDistance(totalDistance);

  pumpDisable();
  esp_deep_sleep_start();
}

void Controller::pumpEnable() {
  if (m_pumpPtr->isEnabled()) {
    return;
  }

  m_pumpPtr->enable();

  ESP_LOGI("Controller", "Pump enabled");

  uint64_t const pumpTimeout_InSeconds = m_configuration->getPumpTimeout();
  uint64_t const pumpTimeout_InMicroseconds = pumpTimeout_InSeconds * PER_MICROSECOND;

  m_timerPtr->start(pumpTimeout_InMicroseconds, [this] {
    m_configuration->setLubricate(false);

    float const actualDistance = m_wheelSensorPtr->getDistance();
    float const savedDistance = m_configuration->getTotalDistance();
    float const totalDistance = savedDistance + actualDistance;
    float const distanceForEnable = m_configuration->getDistanceForEnable();
    float const nextDistance = totalDistance + distanceForEnable;

    m_configuration->saveNextDistance(nextDistance);

    pumpDisable();
  });
}

void Controller::pumpDisable() {
  if (not m_pumpPtr->isEnabled()) {
    return;
  }

  m_pumpPtr->disable();

  ESP_LOGI("Controller", "Pump disabled");

  m_timerPtr->stop();
}
