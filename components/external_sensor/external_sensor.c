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
// Created by jadjer on 03.02.23.
//

#include "external_sensor.h"

#include <driver/gpio.h>
#include <esp_log.h>

#define EXTERNAL_SENSOR_PIN CONFIG_EXTERNAL_SENSOR_PIN
#define EXTERNAL_SENSOR_SEL (1ULL<<EXTERNAL_SENSOR_PIN)

static const char* TAG = "external sensor";

static bool is_init = false;

static void gpio_isr_handler(void *arg) {
    printf("external pin interrupt");
}

esp_err_t external_sensor_init(void) {
    esp_err_t err = ESP_OK;

    if (is_init) {
        return err;
    }

    gpio_config_t io_conf = {
            .intr_type = GPIO_INTR_LOW_LEVEL,
            .pin_bit_mask = EXTERNAL_SENSOR_SEL,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE
    };

    err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        return err;
    }

//    err = gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
//    if (err != ESP_OK) {
//        return err;
//    }

//    err = gpio_isr_handler_add(EXTERNAL_POWER_PIN, gpio_isr_handler, NULL);
//    if (err != ESP_OK) {
//        return err;
//    }

    is_init = true;
    return err;
}

uint8_t extenal_sensor_is_enabled(void) {
    if (!is_init) {
        ESP_LOGW(TAG, "External power does not init");
    }

    int is_powered = gpio_get_level(EXTERNAL_SENSOR_PIN);
    return is_powered;
}

//void sleep(void) {
//
//}

