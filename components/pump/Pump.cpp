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
#include <chrono>

constexpr auto tag = "Pump";

Pump::Pump() :
        m_state(PumpState::PUMP_DISABLED),
        m_controlPin(static_cast<gpio_num_t>(CONFIG_PUMP_CONTROL_PIN)),
        m_feedbackPin(static_cast<gpio_num_t>(CONFIG_PUMP_FEEDBACK_PIN)),
        m_delay(60s),
        m_startTime(std::chrono::system_clock::now()) {

    gpio_config_t ioConf = {
            .pin_bit_mask = (1ULL << m_controlPin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&ioConf));
}

Pump::~Pump() = default;

[[maybe_unused]] PumpState Pump::getState() const {
    return m_state;
}

bool Pump::isEnabled() const {
    return m_state == PUMP_ENABLED;
}

bool Pump::isDisabled() const {
    return m_state == PUMP_DISABLED;
}

bool Pump::inError() const {
    return m_state == PUMP_IN_ERROR;
}

void Pump::enable(std::chrono::seconds delay) {
    if (m_state != PUMP_DISABLED) { return; }

    m_state = PUMP_ENABLED;
    m_delay = delay;

    ESP_ERROR_CHECK(gpio_set_level(m_controlPin, 1));
    ESP_LOGI(tag, "Is enabled");

#if !CONFIG_PUMP_FEEDBACK_IGNORE
    auto feedback = gpio_get_level(m_feedbackPin);
    if (feedback == 0) {
        disable();
        m_state = PUMP_IN_ERROR;
        return;
    }
#endif

    m_startTime = std::chrono::system_clock::now();
}

void Pump::disable() {
    if (m_state != PUMP_ENABLED) { return; }

    m_state = PUMP_DISABLED;

    ESP_ERROR_CHECK(gpio_set_level(m_controlPin, 0));
    ESP_LOGI(tag, "Is disabled");
}

void Pump::spinOnce() {
    if (m_state != PUMP_ENABLED) { return; }



    auto currentTime = std::chrono::system_clock::now();
    auto diffTime = currentTime - m_startTime;
    auto endTime = m_delay - diffTime;
    auto endTimeSec = std::chrono::duration_cast<std::chrono::seconds>(endTime);

    ESP_LOGI(tag, "Pump disabled until %lld second(s)", endTimeSec.count());

    if (endTimeSec.count() <= 0) {
        disable();
    }
}
