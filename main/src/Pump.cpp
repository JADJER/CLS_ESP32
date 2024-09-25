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
// Created by jadjer on 01.10.23.
//

#include "Pump.hpp"

#include "gpio/OutputPin.hpp"

Pump::Pump(uint8_t const numberOfPin) : m_enable(false),
                                        m_pumpPin(std::make_unique<gpio::OutputPin>(numberOfPin)) {
}

bool Pump::isEnabled() const {
  return m_enable;
}

void Pump::enable() {
  if (isEnabled()) {
    return;
  }

  m_pumpPin->setLevel(gpio::PIN_LEVEL_HIGH);

  m_enable = true;
}

void Pump::disable() {
  if (not isEnabled()) {
    return;
  }

  m_pumpPin->setLevel(gpio::PIN_LEVEL_LOW);

  m_enable = false;
}
