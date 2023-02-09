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

#include "Pump.hpp"

#include <driver/gpio.h>
#include <esp_log.h>

constexpr auto tag = "pump";
constexpr auto pumpPin = static_cast<gpio_num_t>(CONFIG_PUMP_POWER_PIN);
constexpr auto pumpPinSel = (1ULL << pumpPin);

static bool isInit = false;

esp_err_t Pump::init() {
    ESP_LOGI(tag, "==================================================");

    esp_err_t err = ESP_OK;

    if (isInit) {
        return err;
    }

    ESP_LOGI(tag, "Initializing...");

    gpio_config_t ioConf = {
            .pin_bit_mask = pumpPinSel,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    err = gpio_config(&ioConf);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(tag, "Initialized");

    isInit = true;
    return err;
}

Pump::State Pump::getState() {
    if (not isInit) {
        ESP_LOGW(tag, "Pump does not init");
    }

    return Pump::PUMP_IN_ERROR;
}

void Pump::enable() {
    if (not isInit) {
        ESP_LOGW(tag, "Pump does not init");
        return;
    }

    gpio_set_level(pumpPin, true);
}

void Pump::disable() {
    if (not isInit) {
        ESP_LOGW(tag, "Pump does not init");
        return;
    }

    gpio_set_level(pumpPin, false);
}
