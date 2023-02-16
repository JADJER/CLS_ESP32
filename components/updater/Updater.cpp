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
// Created by jadjer on 14.02.23.
//

#include "updater/Updater.hpp"

#include <esp_log.h>
#include <esp_ota_ops.h>

constexpr auto tag = "Updater";

Updater::Updater() = default;

Updater::~Updater() = default;

[[maybe_unused]] void Updater::currentPartitionPrint() const {
    auto partition = esp_ota_get_running_partition();
    auto address = partition->address;
    switch (address) {
        case 0x00010000:
            ESP_LOGI(tag, "Running partition: factory");
            break;
        case 0x00110000:
            ESP_LOGI(tag, "Running partition: ota_0");
            break;
        case 0x00210000:
            ESP_LOGI(tag, "Running partition: ota_1");
            break;
        default:
            ESP_LOGE(tag, "Running partition: unknown (%lu)", address);
            break;
    }
}

void Updater::updateApply() {
    auto partition = esp_ota_get_running_partition();

    esp_ota_img_states_t ota_state;

    if (esp_ota_get_state_partition(partition, &ota_state) != ESP_OK) { return; }
    if (ota_state != ESP_OTA_IMG_PENDING_VERIFY) { return; }

    ESP_LOGI(tag, "An OTA update has been detected.");
    ESP_LOGI(tag, "Diagnostics completed successfully! Continuing execution.");
    esp_ota_mark_app_valid_cancel_rollback();

    //    if (run_diagnostics()) {
    //
    //    } else {
    //        ESP_LOGE(tag, "Diagnostics failed! Start rollback to the previous version.");
    //        esp_ota_mark_app_invalid_rollback_and_reboot();
    //    }
}
