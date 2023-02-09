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

#include "Controller.hpp"

#include "PowerManager.hpp"
#include "Pump.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task_wdt.h>

constexpr auto taskResetPeriodMS = 2000;

[[noreturn]] void loopHandler(void *arg) {
    ESP_ERROR_CHECK(esp_task_wdt_add(nullptr));
    ESP_ERROR_CHECK(esp_task_wdt_status(nullptr));

    while (true) {
        esp_task_wdt_reset();

        if (PowerManager::isEnabled()) {
            Pump::enable();
        } else {
            Pump::disable();
        }

        vTaskDelay(pdMS_TO_TICKS(taskResetPeriodMS));
    }
}

esp_err_t Controller::init() {
    xTaskCreate(loopHandler, "controller_loop", 2048, nullptr, 1, nullptr);

    return ESP_OK;
}
