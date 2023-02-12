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

#include "PowerManager.hpp"

#include <esp_log.h>
#include <esp_sleep.h>

constexpr auto tag = "Power manager";

PowerManager::PowerManager() :
        m_powerPin(static_cast<gpio_num_t>(CONFIG_EXTERNAL_POWER_PIN)),
        m_powerPinEnableLevel(1) {

    gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << m_powerPin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_LOW_LEVEL,
    };

#ifdef CONFIG_EXTERNAL_POWER_PIN_INVERTED
    m_powerPinEnableLevel = 0;
#endif

    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(m_powerPin, m_powerPinEnableLevel));
}

PowerManager::~PowerManager() = default;

bool PowerManager::isEnabled() const {
    auto powerLevel = gpio_get_level(m_powerPin);
    if (powerLevel == m_powerPinEnableLevel) {
        ESP_LOGI(tag, "External power is ENABLED");
        return true;
    }

    ESP_LOGI(tag, "External power is DISABLED");
    return false;
}

bool PowerManager::isDisabled() const {
    return not isEnabled();
}
