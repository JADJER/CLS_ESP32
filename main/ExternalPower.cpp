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

#include "ExternalPower.hpp"

#include <cassert>

#include "gpio/InputPin.hpp"

ExternalPower::ExternalPower(uint8_t const numberOfPin, PinState const defaultLevel) :
    m_externalPowerPin(std::make_unique<gpio::InputPin>(numberOfPin, defaultLevel))
{
    assert(EXTERNAL_POWER_COUNT == 2);
}

void ExternalPower::setCallback(ExternalPowerCallbackFunction const& callback)
{
    m_callback = callback;
}

void ExternalPower::process()
{
    if (not m_callback)
    {
        return;
    }

    auto const externalPowerLevel = m_externalPowerPin->getLevel();

    if (externalPowerLevel == gpio::PIN_LEVEL_HIGH)
    {
        m_callback(EXTERNAL_POWER_OFF);
    }

    if (externalPowerLevel == gpio::PIN_LEVEL_LOW)
    {
        m_callback(EXTERNAL_POWER_ON);
    }
}
