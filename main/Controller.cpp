//
// Created by jadjer on 3/18/24.
//

#include "Controller.hpp"

#include <esp_log.h>
#include <esp_sleep.h>

const char *TAG = "Controller";

Controller::Controller() :
    m_pumpPtr(std::make_unique<Pump>()),
    m_timerPtr(std::make_unique<Timer>()),
    m_speedSensorPtr(std::make_unique<SpeedSensor>()),
    m_externalPowerPtr(std::make_unique<ExternalPower>()) {

}

void Controller::process() {
  if (not m_externalPowerPtr->isEnabled()) {
    ESP_LOGI(TAG, "External power is disabled. Sleeping");
    m_pumpPtr->disable();
    m_timerPtr->stop();
    sleep();
  }

  m_timerPtr->start(std::chrono::minutes(20));

  auto const speed = m_speedSensorPtr->getSpeed();

  if (m_timerPtr->isCompleted()) {
    m_pumpPtr->enable();
  }
}

void Controller::sleep() {
  esp_deep_sleep_start();
}
