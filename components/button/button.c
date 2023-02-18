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
// Created by jadjer on 13.02.23.
//

#include "button/button.h"

#include <esp_log.h>
#include <driver/gpio.h>

#define TAG "Button"

const gpio_num_t m_button_pin = CONFIG_BUTTON_PIN;

static uint8_t m_button_pin_enable_level = 0;

esp_err_t button_init(void) {
    gpio_config_t ioConf = {
            .pin_bit_mask = (1ULL << m_button_pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    esp_err_t err = ESP_OK;

    err = gpio_config(&ioConf);
    if (err) {
        return err;
    }

    return err;
}

uint8_t button_is_pressed(void) {
    uint8_t buttonLevel = gpio_get_level(m_button_pin);
    if (buttonLevel == m_button_pin_enable_level) {
        ESP_LOGI(TAG, "It's PRESSED");
        return 1;
    }

    return 0;
}
