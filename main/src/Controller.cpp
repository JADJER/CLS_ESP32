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
                                                         m_externalPowerPtr(std::make_unique<ExternalPower>(m_configuration->getExternalPowerPin())),

                                                         m_pumpEnable(false) {
}

void Controller::spinOnce() {
  if (not m_externalPowerPtr->isEnabled()) {
    sleep();
  }

  auto const speed = m_wheelSensorPtr->getSpeed();
  auto const actualDistance = m_wheelSensorPtr->getDistance();
  auto const savedDistance = m_configuration->getTotalDistance();
  auto const totalDistance = savedDistance + actualDistance;
  auto const distanceForEnable = m_configuration->getDistanceForEnable();
  auto const nextDistance = totalDistance + distanceForEnable;
  auto const isLubricate = m_pumpEnable ? "True" : "False";

  ESP_LOGI("Controller", "Total distance: %f[m], next distance: %f[m], speed: %f[m/s], lubricate: %s", totalDistance, nextDistance, speed, isLubricate);

  if (speed < m_configuration->getMinimalSpeed()) {
    pumpDisable();
    return;
  }

  if (totalDistance >= nextDistance) {
    m_pumpEnable = true;
  }

  if (m_pumpEnable) {
    pumpEnable();
  }
}

void Controller::sleep() {
  ESP_LOGI("Controller", "External power is disabled. Sleeping");

  auto const actualDistance = m_wheelSensorPtr->getDistance();
  auto const savedDistance = m_configuration->getTotalDistance();
  auto const totalDistance = savedDistance + actualDistance;

  m_configuration->saveTotalDistance(totalDistance);

  pumpDisable();
  esp_deep_sleep_start();
}

void Controller::pumpEnable() {
  m_pumpPtr->enable();

  auto const pumpTimeout_InSeconds = m_configuration->getPumpTimeout();
  auto const pumpTimeout_InMicroseconds = pumpTimeout_InSeconds * PER_MICROSECOND;
  m_timerPtr->start(pumpTimeout_InMicroseconds, [this] {
    m_pumpEnable = false;

    auto const actualDistance = m_wheelSensorPtr->getDistance();
    auto const savedDistance = m_configuration->getTotalDistance();
    auto const totalDistance = savedDistance + actualDistance;
    auto const distanceForEnable = m_configuration->getDistanceForEnable();
    auto const nextDistance = totalDistance + distanceForEnable;

    m_configuration->saveNextDistance(nextDistance);

    pumpDisable();
  });
}

void Controller::pumpDisable() {
  m_pumpPtr->disable();
}
