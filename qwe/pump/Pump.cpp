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

#include "Pump.hpp"

#include <Arduino.h>

Pump::Pump(int controlPin) {
  m_delay = 5000;
  m_state = PumpState::DISABLE;
  m_controlPin = controlPin;
  m_startTime = 0;

  pinMode(m_controlPin, OUTPUT);
}

Pump::~Pump() = default;

PumpState Pump::getState() const {
  return m_state;
}

bool Pump::isEnabled() const {
  return m_state == PumpState::ENABLE;
}

bool Pump::isPaused() const {
  return m_state == PumpState::PAUSE;
}

bool Pump::isDisabled() const {
  return m_state == PumpState::DISABLE;
}

void Pump::enable(int delay) {
  m_delay = delay;
  enable();
}

void Pump::enable() {
  if (isEnabled()) { return; }

  m_state = PumpState::ENABLE;

  digitalWrite(m_controlPin, HIGH);

  m_startTime = millis();

  log_i("Pump enabled");
}

void Pump::disable() {
  if (isDisabled()) { return; }

  m_state = PumpState::DISABLE;

  digitalWrite(m_controlPin, LOW);

  log_i("Pump disabled");
}

void Pump::pause() {
  if (isDisabled()) { return; }

  disable();

  m_state = PumpState::PAUSE;

  log_i("Pump paused");
}

void Pump::unpause() {
  if (not isPaused()) { return; }

  enable();
}

void Pump::spinOnce() {
  if (isDisabled()) { return; }

  auto currentTime = millis();
  auto diffTime = currentTime - m_startTime;
  if (diffTime > m_delay) {
    disable();
  }
}
