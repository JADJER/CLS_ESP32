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

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task_wdt.h>
#include <esp_sleep.h>

constexpr auto tag = "Controller";
constexpr auto taskResetPeriodMS = 1000;
constexpr auto watchdogTimeoutMS = 5000;

Controller::Controller(std::shared_ptr<IConfiguration> const &configuration) :
        m_pump(std::make_unique<Pump>()),
        m_timer(std::make_unique<Timer>()),
        m_button(std::make_unique<Button>()),
        m_updater(std::make_unique<Updater>()),
        m_distance(std::make_unique<Distance>()),
        m_bluetooth(std::make_unique<Bluetooth>("CLS")),
        m_powerManager(std::make_unique<PowerManager>()) {

    assert(configuration != nullptr);

    m_configuration = configuration;

    esp_task_wdt_config_t wdtConfig = {
            .timeout_ms = watchdogTimeoutMS,
            .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
            .trigger_panic = true,
    };

    esp_task_wdt_init(&wdtConfig);

    m_bluetooth->advertiseConfig();
    m_bluetooth->advertiseStart();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
    ESP_LOGI(tag, "Loop start");

    ESP_ERROR_CHECK(esp_task_wdt_add(nullptr));
    ESP_ERROR_CHECK(esp_task_wdt_status(nullptr));

    m_updater->currentPartitionPrint();
    m_updater->updateApply();

    while (true) {
        ESP_ERROR_CHECK(esp_task_wdt_reset());

        spinOnce();
        vTaskDelay(pdMS_TO_TICKS(taskResetPeriodMS));
    }

    ESP_LOGI(tag, "Loop stop");
}

void Controller::spinOnce() {
    sleep();

    pumpManual();

    lubricateFromDistance();
    lubricateFromTimer();

    m_timer->spinOnce();
//    m_bluetooth->spinOnce();
}

void Controller::sleep() {
    if (m_pump->isEnabled()) { return; }
    if (m_powerManager->isEnabled()) { return; }

    ESP_LOGI(tag, "Sleep");
    esp_deep_sleep_start();
}

void Controller::lubricateFromDistance() {
    if (not m_configuration->isLubricateFromDistance()) { return; }

    auto currentDistance = m_distance->getDistance();
    auto lubricateDistance = m_configuration->getLimitDistance();

    if (currentDistance >= lubricateDistance) {
        pumpStart();
    }
}

void Controller::lubricateFromTimer() {
    if (not m_configuration->isLubricateFromTimer()) { return; }
    if (m_timer->isEnabled()) { return; }

    auto pumpEnableDelay = m_configuration->getPumpTimeoutEnable();

    m_timer->stop();
    m_timer->setCompleteCallback([this] { pumpStart(); });
    m_timer->start(pumpEnableDelay);
}

void Controller::pumpStart() {
    if (m_pump->isEnabled()) { return; }

    auto pumpDisableDelay = m_configuration->getPumpTimeoutDisable();

    m_pump->enable();

    m_timer->stop();
    m_timer->setCompleteCallback([this] { pumpStop(); });
    m_timer->start(pumpDisableDelay);
}

void Controller::pumpStop() {
    if (m_pump->isDisabled()) { return; }

    m_pump->disable();

    m_timer->stop();
}

void Controller::pumpManual() {
    if (m_button->isPressed()) {
        pumpStart();
    }
}
