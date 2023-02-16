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
// Created by jadjer on 06.02.23.
//

#pragma once

#include <driver/gpio.h>
#include <esp_err.h>

/**
 * @enum PumpState
 */
enum PumpState {
    PUMP_DISABLED,
    PUMP_ENABLED,
    PUMP_IN_ERROR,
};

/**
 * @class Pump
 */
class Pump {
   public:
    /**
     * @brief Default constructor
     */
    Pump();
    /**
     * @brief Default destructor
     */
    ~Pump();

   public:
    /**
     * @brief Pump power enable
     */
    void enable();
    /**
     * @brief Pump power disable
     */
    void disable();

   public:
    /**
     * @brief Get current pump state
     * @return PumpState
     */
    [[nodiscard]] PumpState getState() const;
    /**
     * Is pump enabled
     * @return True if enabled, otherwise false
     */
    [[nodiscard]] bool isEnabled() const;
    /**
     * Is pump disabled
     * @return True if disabled, otherwise false
     */
    [[nodiscard]] bool isDisabled() const;
    /**
     * Is pump in error
     * @return True if in error, otherwise false
     */
    [[nodiscard]] bool inError() const;

   private:
    PumpState m_state;
    gpio_num_t m_controlPin;
    gpio_num_t m_feedbackPin;
};
