//
// Created by jadjer on 10/3/24.
//

#pragma once

#include <cstdint>

#include "NimBLECharacteristic.h"
#include "Updater.hpp"

class MessageHandler {
public:
  MessageHandler(NimBLECharacteristic *dataCharacteristic, NimBLECharacteristic *commandCharacteristic);

public:
  void dataHandle(std::uint8_t const *data, std::size_t length);
  void commandHandle(std::uint8_t const *data, std::size_t length);

private:
  Updater *m_updater = nullptr;
  NimBLECharacteristic *m_dataCharacteristic = nullptr;
  NimBLECharacteristic *m_commandCharacteristic = nullptr;

private:
  std::uint8_t *m_firmwareBuffer = nullptr;

private:
  std::uint32_t m_currentSector = 0;
  std::uint32_t m_currentPacket = 0;
  std::uint32_t m_otaTotalLength = 0;
  std::uint32_t m_firmwareBufferOffset = 0;
};

#include <memory>

using MessageHandlerPtr = std::unique_ptr<MessageHandler>;
