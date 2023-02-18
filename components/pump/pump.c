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

#include "pump/pump.h"

#include <esp_log.h>
#include <driver/gpio.h>

#define TAG "Pump"

const gpio_num_t m_power_pin = CONFIG_PUMP_CONTROL_PIN;
const gpio_num_t m_feedback_pin = CONFIG_PUMP_FEEDBACK_PIN;

static enum PumpState m_state = PUMP_DISABLED;

esp_err_t pump_init(void) {
    gpio_config_t power_pin_conf = {
            .pin_bit_mask = (1ULL << m_power_pin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config_t feedback_pin_conf = {
            .pin_bit_mask = (1ULL << m_feedback_pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    esp_err_t ret = ESP_OK;

    ret = gpio_config(&power_pin_conf);
    if (ret) {
        return ret;
    }

    ret = gpio_config(&feedback_pin_conf);
    if (ret) {
        return ret;
    }

    return ret;
}

void pump_enable(void) {
    if (m_state != PUMP_DISABLED) { return; }

    m_state = PUMP_ENABLED;

    ESP_ERROR_CHECK(gpio_set_level(m_power_pin, 1));
    ESP_LOGI(TAG, "Is enabled");

#if !CONFIG_PUMP_FEEDBACK_IGNORE
    uint8_t feedback = gpio_get_level(m_feedback_pin);
    if (feedback == 1) { return; }

    pump_disable();

    m_state = PUMP_IN_ERROR;

    ESP_LOGI(TAG, "In error");
#endif
}

void pump_disable(void) {
    if (m_state != PUMP_ENABLED) { return; }

    m_state = PUMP_DISABLED;

    ESP_ERROR_CHECK(gpio_set_level(m_power_pin, 0));
    ESP_LOGI(TAG, "Is disabled");
}

enum PumpState pump_get_state(void) {
    return m_state;
}

uint8_t pump_is_enabled(void) {
    return m_state == PUMP_ENABLED;
}

uint8_t pump_is_disabled(void) {
    return m_state == PUMP_DISABLED;
}

uint8_t pump_in_error(void) {
    return m_state == PUMP_IN_ERROR;
}
