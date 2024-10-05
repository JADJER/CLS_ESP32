//
// Created by jadjer on 10/2/24.
//

#include "Updater.hpp"

#include <esp_log.h>

constexpr auto const TAG = "Update";

Updater::Updater() : m_otaPartition(esp_ota_get_next_update_partition(nullptr)) {
  esp_ota_mark_app_valid_cancel_rollback();

  if (not isAvailable()) {
    ESP_LOGE(TAG, "OTA partition not available");
  }
}

bool Updater::isAvailable() const {
  return m_otaPartition == nullptr;
}

bool Updater::isStarted() const {
  if (not isAvailable()) {
    return false;
  }

  return m_start;
}

void Updater::begin(std::size_t const otaLength) {
  if (not isAvailable()) {
    return;
  }

  if (isStarted()) {
    ESP_LOGW(TAG, "Update already begin...");
    return;
  }

  m_start = true;

  //  esp_err_t err = esp_ota_begin(m_otaPartition, otaLength, &m_otaHandle);
  //  if (err != ESP_OK) {
  //    ESP_LOGE(TAG, "esp_ota_begin failed!\r\n");
  //    abort();
  //  }
}

void Updater::writeData(std::uint8_t const *data, std::size_t const dataLength) {
  if (not isAvailable()) {
    return;
  }

  if (not isStarted()) {
    ESP_LOGE(TAG, "Not started");
    return;
  }

  //  esp_err_t err = esp_ota_write(m_otaHandle, static_cast<void const *>(data), dataLength);
  //  if (err != ESP_OK) {
  //    ESP_LOGE(TAG, "esp_ota_write failed!\r\n");
  //    abort();
  //  }
}

void Updater::end() {
  if (not isAvailable()) {
    return;
  }

  if (not isStarted()) {
    ESP_LOGE(TAG, "Not started");
    return;
  }

  //  esp_err_t err = esp_ota_end(m_otaHandle);
  //  if (err != ESP_OK) {
  //    ESP_LOGE(TAG, "esp_ota_end failed!\r\n");
  //    abort();
  //  }

  m_start = false;
}

void Updater::install() {
  if (not isAvailable()) {
    return;
  }

  if (not isStarted()) {
    ESP_LOGE(TAG, "Not started");
    return;
  }

  //  esp_err_t err = esp_ota_set_boot_partition(m_otaPartition);
  //  if (err != ESP_OK) {
  //    ESP_LOGE(TAG, "esp_ota_set_boot_partition failed!\r\n");
  //    abort();
  //    return;
  //  }
}

void Updater::abort() {
  if (not isAvailable()) {
    return;
  }

  //  esp_err_t err = esp_ota_abort(m_otaHandle);
  //  if (err != ESP_OK) {
  //    ESP_LOGE(TAG, "esp_ota_abort failed!\r\n");
  //    return;
  //  }

  m_start = false;
}
