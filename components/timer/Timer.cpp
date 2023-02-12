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

#include "Timer.hpp"

#include <esp_log.h>

constexpr auto tag = "Timer";

Timer::Timer() :
        m_isEnabled(false),
        m_isCompleted(false),
        m_delay(60s),
        m_startTime(std::chrono::system_clock::now()) {}

Timer::~Timer() = default;

void Timer::start(std::chrono::seconds delay) {
    if (m_isEnabled) { return; }

    m_isEnabled = true;

    m_delay = delay;
    m_startTime = std::chrono::system_clock::now();
    m_isCompleted = false;
}

void Timer::stop() {
    if (not m_isEnabled) { return; }

    m_isEnabled = false;
}

bool Timer::isEnabled() const {
    return false;
}


bool Timer::isCompleted() const {
    return m_isCompleted;
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
        m_isCompleted = true;
    }
}
