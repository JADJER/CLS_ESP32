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

#include <functional>

#include "executor/Node.hpp"
#include "gpio/interface/IPin.hpp"

using PinState = gpio::PinLevel;
using DistanceSensorCallbackFunction = std::function<void(float)>;

class DistanceSensor : public executor::Node
{
public:
    DistanceSensor();
    ~DistanceSensor() override = default;

public:
    void setCallback(DistanceSensorCallbackFunction const& callback);

private:
    void process() override;

private:
    DistanceSensorCallbackFunction m_callback = nullptr;

private:
    float m_distance;
    IPinPtr m_distanceSensorPin;
    PinState m_distanceSensorState;
};
