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
// Created by jadjer on 09.02.23.
//


#pragma once

#include <esp_err.h>
#include <driver/gpio.h>

/**
 * @class PowerManager
 */
class PowerManager {
public:
    /**
     * Default constructor
     */
    PowerManager();
    /**
     * Default destructor
     */
    ~PowerManager();

public:
    /**
     * Check if power is enabled
     * @return True if enabled. false is disabled
     */
    [[nodiscard]] bool isEnabled() const;
    /**
     * Check if power is disabled
     * @return True if disabled. false is enabled
     */
    [[nodiscard]] bool isDisabled() const;

private:
    gpio_num_t m_powerPin;
    uint8_t m_powerPinEnableLevel;
};
