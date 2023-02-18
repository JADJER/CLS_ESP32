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

#include "power_manager/power_manager.h"

#include <esp_log.h>
#include <esp_sleep.h>
#include <driver/gpio.h>

#define TAG "Power manager"

const gpio_num_t m_external_power_pin = CONFIG_EXTERNAL_POWER_PIN;

static uint8_t m_power_pin_enable_level = 1;

esp_err_t power_manager_init(void) {
    gpio_config_t external_power_pin_conf = {
            .pin_bit_mask = (1ULL << m_external_power_pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_LOW_LEVEL,
    };

#if defined(CONFIG_EXTERNAL_POWER_PIN_INVERTED)
    m_power_pin_enable_level = CONFIG_EXTERNAL_POWER_PIN_INVERTED;
#endif

    esp_err_t ret = ESP_OK;

    ret = gpio_config(&external_power_pin_conf);
    if (ret != ESP_OK) {
        return ret;
    }

    ret = esp_sleep_enable_ext0_wakeup(m_external_power_pin, m_power_pin_enable_level);
    if (ret != ESP_OK) {
        return ret;
    }

    return ret;
}

uint8_t power_manager_is_enabled(void) {
    uint8_t power_level = gpio_get_level(m_external_power_pin);
    if (power_level == m_power_pin_enable_level) {
        ESP_LOGI(TAG, "External power is ENABLED");
        return 1;
    }

    ESP_LOGI(TAG, "External power is DISABLED");
    return 0;
}
