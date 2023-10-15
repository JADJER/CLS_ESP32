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

#pragma once

#include <chrono>

#include "executor/Node.hpp"
#include "gpio/PinLevel.hpp"
#include "gpio/interface/IOutputPin.hpp"

using PinState = gpio::PinLevel;
using MicroSeconds = std::chrono::microseconds;
using TimePoint = std::chrono::system_clock::time_point;

class Pump : public executor::Node
{
public:
    Pump();
    ~Pump() override = default;

public:
    void enable(MicroSeconds delay);
    void disable();

private:
    void process() override;

private:
    bool m_enable;
    MicroSeconds m_delay;
    TimePoint m_startTime;
    IOutputPinPtr<PinState> m_pumpPin;
};
