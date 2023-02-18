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

#include "timer/timer.h"

#include <esp_log.h>
#include <time.h>

#define TAG "Timer"

static uint8_t m_is_enabled = 0;
static timer_complete_callback_t m_callback;
static uint32_t m_delay = 60;
static time_t m_start_time;

void timer_set_complete_callback(timer_complete_callback_t callback) {
    m_callback = callback;
}

void timer_start(uint32_t delay) {
    if (m_is_enabled) { return; }

    m_is_enabled = 1;

    m_delay = delay;
    m_start_time = time(NULL);

    ESP_LOGI(TAG, "Start for %lu second(s)", m_delay);
}

void timer_stop(void) {
    if (!m_is_enabled) { return; }

    m_is_enabled = 0;

    ESP_LOGI(TAG, "Stop");
}

uint8_t timer_is_enabled(void) {
    return m_is_enabled;
}

void timer_spin_once() {
    if (!m_is_enabled) { return; }

    time_t current_time = time(NULL);
    time_t diff_time = current_time - m_start_time;
    time_t end_time = m_delay - diff_time;

    ESP_LOGI(TAG, "Completed until %lld second(s)", end_time);

    if (end_time <= 0) {
        m_is_enabled = 0;

        m_callback();
    }
}
