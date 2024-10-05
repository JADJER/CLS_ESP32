//
// Created by jadjer on 10/2/24.
//

#pragma once

#include <cstdint>
#include <esp_ota_ops.h>

class Updater {
public:
  Updater();
  ~Updater() = default;

public:
  [[nodiscard]] bool isAvailable() const;
  [[nodiscard]] bool isStarted() const;

public:
  void begin(std::size_t otaLength);
  void writeData(std::uint8_t const *data, std::size_t dataLength);
  void end();
  void install();
  void abort();

private:
  bool m_start = false;
  esp_ota_handle_t m_otaHandle = 0;
  esp_partition_t const *m_otaPartition = nullptr;
};
