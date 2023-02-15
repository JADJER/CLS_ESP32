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

#include "configuration/Configuration.hpp"

#include <nvs.h>
#include <nvs_flash.h>

constexpr auto tag = "Configuration";

Configuration::Configuration() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES or err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

Configuration::~Configuration() = default;

bool Configuration::isLubricateFromTimer() const {
    return true;
}

bool Configuration::isLubricateFromDistance() const {
    return true;
}

uint64_t Configuration::getLimitDistance() const {
    return 500;
}

std::chrono::seconds Configuration::getPumpTimeoutEnable() const {
    return std::chrono::hours(1);
}

std::chrono::seconds Configuration::getPumpTimeoutDisable() const {
    return std::chrono::minutes(1);
}
