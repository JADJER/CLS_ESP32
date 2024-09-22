//
// Created by jadjer on 3/18/24.
//

#include "Controller.hpp"

#include <esp_log.h>
#include <esp_pthread.h>
#include <esp_sleep.h>

constexpr uint64_t const PER_MICROSECOND = 1000000;
constexpr uint64_t const WATCHDOG_TIMEOUT_MICROSECOND = 3000;
constexpr uint64_t const TASK_RESET_PERIOD_MICROSECOND = 2000;

Controller::Controller(IConfigurationPtr configuration) : m_configuration(std::move(configuration)),

                                                          m_pumpPtr(std::make_unique<Pump>(m_configuration->getPumpPin())),
                                                          m_timerPtr(std::make_unique<Timer>()),
                                                          m_wheelSensorPtr(std::make_unique<WheelSensor>(m_configuration->getWheelLength(), m_configuration->getWheelSensorPin())),
                                                          m_externalPowerPtr(std::make_unique<ExternalPower>(m_configuration->getExternalPowerPin())),

                                                          m_watchdogHandle(nullptr) {
}

[[noreturn]] void Controller::spin() {
#if not CONFIG_ESP_TASK_WDT_INIT
  esp_task_wdt_config_t watchdogConfig = {
      .timeout_ms = WATCHDOG_TIMEOUT_MICROSECOND,
      .idle_core_mask = 0,
      .trigger_panic = false,
  };
  ESP_ERROR_CHECK(esp_task_wdt_init(&watchdogConfig));
#endif

  ESP_ERROR_CHECK(esp_task_wdt_add_user("controller_spin", &m_watchdogHandle));

  auto const delayAfterBoot_InSeconds = m_configuration->getDelayAfterBoot();
  auto const delayAfterBoot_InMicroseconds = delayAfterBoot_InSeconds * PER_MICROSECOND;
  m_timerPtr->start(delayAfterBoot_InMicroseconds, [] {});

  ESP_LOGI("Controller", "Delay %llu seconds after boot", delayAfterBoot_InSeconds);

  while (true) {
    ESP_ERROR_CHECK(esp_task_wdt_reset_user(m_watchdogHandle));
    spinOnce();
    vTaskDelay(pdMS_TO_TICKS(TASK_RESET_PERIOD_MICROSECOND));
  }

  ESP_ERROR_CHECK(esp_task_wdt_delete_user(m_watchdogHandle));
}

void Controller::spinOnce() {
  //    if (not m_externalPowerPtr->isEnabled()) {
  //      sleep();
  //    }

  if (m_pumpPtr->isEnabled()) {
    return;
  }

  auto const timeForEnable_InSeconds = m_configuration->getTimeForEnable();
  auto const timeForEnable_InMicroseconds = timeForEnable_InSeconds * PER_MICROSECOND;
  m_timerPtr->start(timeForEnable_InMicroseconds, [this] {
    pumpEnable();
  });

  auto const distance = m_wheelSensorPtr->getDistance();
  if (distance >= m_configuration->getDistanceForEnable()) {
    pumpEnable();
  }
}

void Controller::sleep() {
  ESP_LOGI("Controller", "External power is disabled. Sleeping");

  pumpDisable();
  esp_deep_sleep_start();
}

void Controller::pumpEnable() {
  m_pumpPtr->enable();

  auto const pumpTimeout_InSeconds = m_configuration->getPumpTimout();
  auto const pumpTimeout_InMicroseconds = pumpTimeout_InSeconds * PER_MICROSECOND;
  m_timerPtr->start(pumpTimeout_InMicroseconds, [this] {
    pumpDisable();
  });
}

void Controller::pumpDisable() {
  m_pumpPtr->disable();
}
