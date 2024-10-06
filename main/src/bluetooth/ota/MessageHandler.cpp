//
// Created by jadjer on 10/3/24.
//

#include "MessageHandler.hpp"

#include "esp_log.h"

auto constexpr TAG = "MessageHandler";

auto constexpr OTA_BLOCK_SIZE = 4096;
auto constexpr OTA_LAST_PACKET = 0xFF;
auto constexpr OTA_LAST_SECTOR = 0xFFFF;
auto constexpr OTA_COMMAND_LENGTH = 20;
auto constexpr OTA_BLOCK_EMPTY_VALUE = 0x00;

MessageHandler::MessageHandler(CharacteristicPtr dataCharacteristic, CharacteristicPtr commandCharacteristic) : m_updater(std::make_unique<Updater>()),
                                                                                                                m_dataCharacteristic(dataCharacteristic),
                                                                                                                m_commandCharacteristic(commandCharacteristic),
                                                                                                                m_firmwareBuffer(new std::uint8_t[OTA_BLOCK_SIZE]) {
}

void MessageHandler::dataHandle(std::uint8_t const *data, std::size_t const dataLength) {
  auto const answer = [this](std::uint16_t const sector, BinaryAck const status) {
    uint8_t commandAnswer[OTA_COMMAND_LENGTH] = {OTA_BLOCK_EMPTY_VALUE * OTA_COMMAND_LENGTH};

    commandAnswer[0] = sector & 0xff;
    commandAnswer[1] = (sector & 0xff00) >> 8;

    commandAnswer[2] = status & 0xff;
    commandAnswer[3] = (status & 0xff00) >> 8;

    m_dataCharacteristic->indicate(commandAnswer, 4);
  };

  auto const serviceDataLength = 3;

  if (dataLength < serviceDataLength) {
    ESP_LOGE(TAG, "recv ota data wrong length");
    return;
  }

  auto const sector = (data[0]) + (data[1] * 256);

  if (sector != m_currentSector) {
    if (sector != OTA_LAST_SECTOR) {
      ESP_LOGE(TAG, "%s - sector index error, cur: %" PRIu32 ", recv: %d", __func__, m_currentSector, sector);
      answer(sector, BINARY_ACK_SECTOR_INDEX_ERROR);
      return;
    }
  }

  auto const packet = data[2];

  if (packet != m_currentPacket) {
    if (packet != OTA_LAST_PACKET) {
      ESP_LOGE(TAG, "%s - packet index error, cur: %" PRIu32 ", recv: %d", __func__, m_currentPacket, packet);
      return;
    }
  }

  auto const minimalDataLength = 1;

  if (dataLength < serviceDataLength + minimalDataLength) {
    ESP_LOGE(TAG, "recv ota data wrong length");
    answer(sector, BINARY_ACK_PAYLOAD_LENGTH_ERROR);
    return;
  }

  auto const payload = data + serviceDataLength;
  auto const payloadLength = dataLength - serviceDataLength;

  std::memcpy(m_firmwareBuffer + m_firmwareBufferLength, payload, payloadLength);
  m_firmwareBufferLength += payloadLength;

  if (packet != OTA_LAST_PACKET) {
    m_currentPacket++;
    return;
  }

  m_updater->writeData(m_firmwareBuffer, m_firmwareBufferLength);

  m_currentSector++;
  m_currentPacket = 0;

  resetBuffer();

  answer(sector, BINARY_ACK_SUCCESS);
}

void MessageHandler::commandHandle(std::uint8_t const *data, std::size_t const dataLength) {
  auto const answer = [this](Command const command, CommandAck const status) {
    uint8_t commandAnswer[OTA_COMMAND_LENGTH] = {OTA_BLOCK_EMPTY_VALUE * OTA_COMMAND_LENGTH};

    commandAnswer[0] = COMMAND_ACK & 0xff;
    commandAnswer[1] = (COMMAND_ACK & 0xff00) >> 8;

    commandAnswer[2] = command & 0xff;
    commandAnswer[3] = (command & 0xff00) >> 8;

    commandAnswer[4] = status & 0xff;
    commandAnswer[5] = (status & 0xff00) >> 8;

    m_commandCharacteristic->indicate(commandAnswer, 6);
  };

  if (dataLength != OTA_COMMAND_LENGTH) {
    ESP_LOGE(TAG, "recv ota start cmd wrong length (%d != %d)", dataLength, OTA_COMMAND_LENGTH);
    return;
  }

  auto const command = (data[0]) + (data[1] * 256);

  if (command == COMMAND_START) {
    m_currentSector = 0;
    m_currentPacket = 0;

    auto const firmwareTotalLength = (data[2]) + (data[3] * 256) + (data[4] * 256 * 256) + (data[5] * 256 * 256 * 256);

    if (not m_updater->begin(firmwareTotalLength)) {
      answer(COMMAND_START, COMMAND_ACK_REFUSE);
      return;
    }

    ESP_LOGI(TAG, "Start OTA. Firmware length %d bytes", firmwareTotalLength);

    resetBuffer();

    answer(COMMAND_START, COMMAND_ACK_ACCEPT);
  }

  if (command == COMMAND_END) {
    m_currentSector = 0;
    m_currentPacket = 0;

    ESP_LOGI(TAG, "End OTA.");

    m_updater->end();

    answer(COMMAND_END, COMMAND_ACK_ACCEPT);
  }
}

void MessageHandler::resetBuffer() {
  m_firmwareBufferLength = 0;

  std::memset(m_firmwareBuffer, OTA_BLOCK_EMPTY_VALUE, OTA_BLOCK_SIZE);
}
