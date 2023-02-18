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
 * Init
 * @return
 */
esp_err_t pump_init(void);

/**
 * @brief Pump power enable
 */
void pump_enable(void);

/**
 * @brief Pump power disable
 */
void pump_disable(void);

/**
 * @brief Get current pump state
 * @return PumpState
 */
enum PumpState pump_get_state(void);

/**
 * Is pump enabled
 * @return True if enabled, otherwise false
 */
uint8_t pump_is_enabled(void);

/**
 * Is pump disabled
 * @return True if disabled, otherwise false
 */
uint8_t pump_is_disabled(void);

/**
 * Is pump in error
 * @return True if in error, otherwise false
 */
uint8_t pump_in_error(void);
