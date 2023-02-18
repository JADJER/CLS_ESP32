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
// Created by jadjer on 10.02.23.
//

#pragma once

#include <stdint-gcc.h>

typedef void (* timer_complete_callback_t)(void);

/**
 * Set callback
 * @param callback
 */
void timer_set_complete_callback(timer_complete_callback_t callback);
/**
 * Timer start
 * @param delay Timer disable after time
 */
void timer_start(uint32_t delay);
/**
 * Timer stop
 */
void timer_stop(void);
/**
 * Is timer enabled
 * @return True if enabled, otherwise false
 */
uint8_t timer_is_enabled(void);
/**
 * @brief Timer control logic spin
 */
void timer_spin_once(void);
