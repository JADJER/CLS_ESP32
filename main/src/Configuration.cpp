// Copyright 2024 Pavel Suprunov
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
// Created by jadjer on 9/20/24.
//

#include "configuration/Configuration.hpp"

#include <cstring>
#include <nvs.h>
#include <nvs_flash.h>

esp_err_t nvs_set_float(nvs_handle_t handle, const char *key, float value) {
  uint64_t buf = 0;

  memcpy(&buf, &value, sizeof(float));

  return nvs_set_u64(handle, key, buf);
}

esp_err_t nvs_get_float(nvs_handle_t handle, const char *key, float *value) {
  uint64_t buf = 0;

  esp_err_t err = nvs_get_u64(handle, key, &buf);
  if (err == ESP_OK) {
    memcpy(value, &buf, sizeof(float));
  }

  return err;
}

Configuration::Configuration() : m_storageHandle(0) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &m_storageHandle));
}

Configuration::~Configuration() {
  ESP_ERROR_CHECK(nvs_commit(m_storageHandle));
  nvs_close(m_storageHandle);
}

uint8_t Configuration::getExternalPowerPin() const {
  return 15;
}

uint8_t Configuration::getPumpPin() const {
  return 16;
}

uint8_t Configuration::getWheelSensorPin() const {
  return 14;
}

uint64_t Configuration::getPumpTimeout() const {
  uint64_t pumpTimeout = 60;

  nvs_get_u64(m_storageHandle, "pump_timeout", &pumpTimeout);

  return pumpTimeout;
}

float Configuration::getWheelLength() const {
  float wheelLength = 1.3;

  nvs_get_float(m_storageHandle, "wheel_length", &wheelLength);

  return wheelLength;
}

float Configuration::getMinimalSpeed() const {
  float minimalSpeed = 5;

  nvs_get_float(m_storageHandle, "minimal_speed", &minimalSpeed);

  return minimalSpeed;
}

float Configuration::getDistanceForEnable() const {
  float distanceForEnable = 500 * 1000;

  nvs_get_float(m_storageHandle, "distance_for_enable", &distanceForEnable);

  return distanceForEnable;
}

float Configuration::getTotalDistance() const {
  float totalDistance = 0;

  nvs_get_float(m_storageHandle, "total_distance", &totalDistance);

  return totalDistance;
}

float Configuration::getNextDistance() const {
  float nextDistance = 0;

  nvs_get_float(m_storageHandle, "next_distance", &nextDistance);

  return nextDistance;
}

void Configuration::setPumpTimeout(uint64_t timeout) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "pump_timeout", timeout));
}

void Configuration::setWheelLength(float wheelLength) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "wheel_length", wheelLength));
}

void Configuration::setMinimalSpeed(float minimalSpeed) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "minimal_speed", minimalSpeed));
}

void Configuration::setDistanceForEnable(float distance) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "distance_for_enable", distance));
}

void Configuration::saveTotalDistance(float distance) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "total_distance", distance));
}

void Configuration::saveNextDistance(float distance) {
  ESP_ERROR_CHECK(nvs_set_float(m_storageHandle, "next_distance", distance));
}
