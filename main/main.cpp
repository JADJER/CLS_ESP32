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

#include "Pump.hpp"
#include "Controller.hpp"
#include "PowerManager.hpp"

#include <esp_task_wdt.h>

constexpr uint32_t watchdogTimeoutMS = 5000;

void watchdogInit() {
    esp_task_wdt_config_t wdtConfig = {
            .timeout_ms = watchdogTimeoutMS,
            .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
            .trigger_panic = true,
    };

    esp_task_wdt_init(&wdtConfig);
}

extern "C" void app_main(void) {
    watchdogInit();

    ESP_ERROR_CHECK(Pump::init());
    ESP_ERROR_CHECK(PowerManager::init());
    ESP_ERROR_CHECK(Controller::init());
}
