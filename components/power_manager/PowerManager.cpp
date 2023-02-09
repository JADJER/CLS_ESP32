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

#include <driver/gpio.h>
#include <esp_log.h>

constexpr auto tag = "power_manager";
constexpr auto powerPin = static_cast<gpio_num_t>(CONFIG_EXTERNAL_POWER_PIN);
constexpr auto powerPinSel = (1ULL << powerPin);

static bool isInit = false;

esp_err_t PowerManager::init() {
    ESP_LOGI(tag, "==================================================");

    esp_err_t err = ESP_OK;

    if (isInit) {
        return err;
    }

    ESP_LOGI(tag, "Initializing...");

    gpio_config_t io_conf = {
            .pin_bit_mask = powerPinSel,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_ANYEDGE,
    };

    err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(tag, "Initialized");

    isInit = true;
    return err;
}

bool PowerManager::isEnabled() {
    if (!isInit) {
        ESP_LOGW(tag, "Power manager does not init");
        return false;
    }

    int is_powered = gpio_get_level(powerPin);

    return !is_powered;
}
