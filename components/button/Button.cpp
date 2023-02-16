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

#include "button/Button.hpp"

#include <driver/gpio.h>
#include <esp_log.h>

constexpr auto tag = "Button";

Button::Button() :
    m_buttonPin(static_cast<gpio_num_t>(CONFIG_BUTTON_PIN)),
    m_buttonPinEnableLevel(1) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << m_buttonPin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

#if CONFIG_BUTTON_PIN_INVERTED
    m_buttonPinEnableLevel = 0;
#endif

    ESP_ERROR_CHECK(gpio_config(&io_conf));
}

Button::~Button() = default;

bool Button::isPressed() const {
    auto buttonLevel = gpio_get_level(m_buttonPin);
    if (buttonLevel == m_buttonPinEnableLevel) {
        ESP_LOGI(tag, "It's PRESSED");
        return true;
    }

    return false;
}
