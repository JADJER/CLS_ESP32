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

#include "configuration/configuration.h"

#include <nvs_flash.h>

#define TAG "Configuration"

esp_err_t configuration_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    return err;
}

uint8_t configuration_is_lubricate_from_timer(void) {
    return 1;
}

uint8_t configuration_is_lubricate_from_distance(void) {
    return 1;
}

uint64_t configuration_get_limit_distance(void) {
    return 500;
}

uint32_t configuration_get_pump_timeout_enable_s(void) {
    return 60 * 60;
}

uint32_t configuration_get_pump_timeout_disable_s() {
    return 60;
}
