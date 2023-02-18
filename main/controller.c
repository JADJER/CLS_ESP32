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

#include "controller.h"

#include "bluetooth.h"
#include "button/button.h"
#include "configuration/configuration.h"
#include "distance/distance.h"
#include "power_manager/power_manager.h"
#include "pump/pump.h"
#include "timer/timer.h"
#include "updater/updater.h"

#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG "Controller"

const uint16_t m_task_reset_period_ms = 1000;
const uint16_t m_watchdog_timeout_ms = 5000;

void pumpStop(void) {
    if (pump_is_disabled()) { return; }

    pump_disable();

    timer_stop();
}

void pumpStart(void) {
    if (pump_is_enabled()) { return; }

    uint32_t pump_disable_delay_s = configuration_get_pump_timeout_disable_s();

    pump_enable();

    timer_stop();
    timer_set_complete_callback(pumpStop);
    timer_start(pump_disable_delay_s);
}

void pumpManual(void) {
    if (button_is_pressed()) {
        pumpStart();
    }
}

void lubricateFromDistance(void) {
    if (!configuration_is_lubricate_from_distance()) { return; }

    uint32_t current_distance = distance_get_distance();
    uint32_t lubricate_distance = configuration_get_limit_distance();

    if (current_distance >= lubricate_distance) {
        pumpStart();
    }
}

void lubricateFromTimer(void) {
    if (!configuration_is_lubricate_from_timer()) { return; }
    if (timer_is_enabled()) { return; }

    uint32_t pump_enable_delay_s = configuration_get_pump_timeout_enable_s();

    timer_stop();
    timer_set_complete_callback(pumpStart);
    timer_start(pump_enable_delay_s);
}

void spinOnce(void) {
    controller_sleep();

    pumpManual();

    lubricateFromDistance();
    lubricateFromTimer();

    timer_spin_once();
}

esp_err_t controller_init(void) {
    esp_task_wdt_config_t wdtConfig = {
            .timeout_ms = m_watchdog_timeout_ms,
            .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
            .trigger_panic = true,
    };

    esp_task_wdt_init(&wdtConfig);

    esp_err_t err = ESP_OK;

    err = button_init();
    if (err) {
        return err;
    }

    err = configuration_init();
    if (err) {
        return err;
    }

    err = power_manager_init();
    if (err) {
        return err;
    }

    err = pump_init();
    if (err) {
        return err;
    }

    err = bluetooth_init();
    if (err) {
        return err;
    }

    return err;
}

void controller_sleep(void) {
    if (pump_is_enabled()) { return; }
    if (power_manager_is_enabled()) { return; }

    ESP_LOGI(TAG, "Sleep");
    esp_deep_sleep_start();
}


void controller_spin(void) {
    ESP_LOGI(TAG, "Loop start");

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_ERROR_CHECK(esp_task_wdt_status(NULL));

    updater_print_current_partition();
    updater_apply_new_update();

    while (true) {
        ESP_ERROR_CHECK(esp_task_wdt_reset());

        spinOnce();
        vTaskDelay(pdMS_TO_TICKS(m_task_reset_period_ms));
    }

    ESP_LOGI(TAG, "Loop stop");
}
