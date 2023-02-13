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

#include "timer/Timer.hpp"

#include <esp_log.h>

constexpr auto tag = "Timer";

Timer::Timer() :
        m_isEnabled(false),
        m_callback(nullptr),
        m_delay(60),
        m_startTime(std::chrono::system_clock::now()) {}

Timer::~Timer() = default;

void Timer::setCompleteCallback(std::function<void()> const &callback) {
    m_callback = callback;
}

void Timer::start(std::chrono::seconds delay) {
    if (m_isEnabled) { return; }

    m_isEnabled = true;

    m_delay = std::chrono::seconds(delay);
    m_startTime = std::chrono::system_clock::now();

    ESP_LOGI(tag, "Start for %lld second(s)", m_delay.count());
}

void Timer::stop() {
    if (not m_isEnabled) { return; }

    m_isEnabled = false;

    ESP_LOGI(tag, "Stop");
}

bool Timer::isEnabled() const {
    return m_isEnabled;
}

void Timer::spinOnce() {
    if (not m_isEnabled) { return; }

    auto currentTime = std::chrono::system_clock::now();
    auto diffTime = currentTime - m_startTime;
    auto endTime = m_delay - diffTime;
    auto endTimeSec = std::chrono::duration_cast<std::chrono::seconds>(endTime);

    ESP_LOGI(tag, "Completed until %lld second(s)", endTimeSec.count());

    if (endTimeSec.count() <= 0) {
        m_isEnabled = false;

        m_callback();
    }
}
