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

Pump::Pump(int controlPin, int feedbackPin) {
  m_delay = 5000;
  m_state = PumpState::DISABLE;
  m_controlPin = controlPin;
  m_feedbackPin = feedbackPin;
  m_startTime = 0;

  pinMode(m_controlPin, OUTPUT);
  pinMode(m_feedbackPin, INPUT_PULLUP);
}

Pump::~Pump() = default;

void Pump::enable(int delay) {
  if (m_state != PumpState::DISABLE) { return; }

  m_delay = delay;
  m_state = PumpState::ENABLE;

  digitalWrite(m_controlPin, HIGH);

  getFeedback();

  m_startTime = millis();
}

void Pump::disable() {
  if (m_state != PumpState::ENABLE) { return; }

  m_state = PumpState::DISABLE;

  digitalWrite(m_controlPin, LOW);
}

PumpState Pump::getState() const {
  return m_state;
}

void Pump::spinOnce() {
  if (m_state != PumpState::ENABLE) { return; }

  getFeedback();

  auto currentTime = millis();
  auto diffTime = currentTime - m_startTime;
  if (diffTime > m_delay) {
    disable();
  }
}

bool Pump::getFeedback() {
  auto feedback = digitalRead(m_feedbackPin);
  if (feedback == HIGH) {
    disable();
    m_state = PumpState::ERROR;
    return false;
  }

  return true;
}
