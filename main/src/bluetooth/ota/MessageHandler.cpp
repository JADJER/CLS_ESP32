//
// Created by jadjer on 10/3/24.
//

#include "MessageHandler.hpp"

#include "utils.hpp"

#include <esp_log.h>

auto constexpr TAG = "MessageHandler";

auto constexpr OTA_BLOCK_SIZE = 4096;
auto constexpr OTA_LAST_PACKET = 0xff;
auto constexpr OTA_LAST_SECTOR = 0xffff;
auto constexpr OTA_BLOCK_EMPTY_VALUE = 0x00;

auto constexpr COMMAND_ANSWER_LENGTH = 20;

MessageHandler::MessageHandler(CharacteristicPtr dataCharacteristic, CharacteristicPtr commandCharacteristic) : m_updater(std::make_unique<Updater>()),
                                                                                                                m_firmwareBuffer(new std::uint8_t[OTA_BLOCK_SIZE]),
                                                                                                                m_dataCharacteristic(dataCharacteristic),
                                                                                                                m_commandCharacteristic(commandCharacteristic) {
}

void MessageHandler::dataHandle(std::uint8_t const *data, std::size_t const length) {
  auto const answer = [this](std::uint16_t const sector, BinaryAck const status) {
    uint8_t commandAnswer[COMMAND_ANSWER_LENGTH] = {OTA_BLOCK_EMPTY_VALUE * COMMAND_ANSWER_LENGTH};

    commandAnswer[0] = sector & 0xff;
    commandAnswer[1] = (sector & 0xff00) >> 8;
    commandAnswer[2] = status & 0xff;
    commandAnswer[3] = (status & 0xff00) >> 8;

    auto const crc16 = calculateCRC16(commandAnswer, 18);
    commandAnswer[18] = crc16 & 0xff;
    commandAnswer[19] = (crc16 & 0xff00) >> 8;

    m_dataCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);
  };

  if (length < 3) {
    ESP_LOGI(TAG, "recv ota data wrong length");
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
  if (packet != m_currentPacket) {  // packet seq error
    if (packet != OTA_LAST_PACKET) {// last packet
      ESP_LOGE(TAG, "%s - packet index error, cur: %" PRIu32 ", recv: %d", __func__, m_currentPacket, packet);
      return;
    }
  }

  if (length < 4) {
    ESP_LOGI(TAG, "recv ota data wrong length");
    answer(sector, BINARY_ACK_PAYLOAD_LENGTH_ERROR);
    return;
  }

  auto const payload = data + 3;
  auto const payloadLength = length - 3;

  std::memcpy(m_firmwareBuffer + m_firmwareBufferOffset, payload, payloadLength);

  m_firmwareBufferOffset += payloadLength;

  if (packet != OTA_LAST_PACKET) {
    m_currentPacket++;
    return;
  }

  m_currentSector++;
  m_currentPacket = 0;

  m_updater->writeData(m_firmwareBuffer, m_firmwareBufferOffset);

  resetBuffer();

  answer(sector, BINARY_ACK_SUCCESS);
}

void MessageHandler::commandHandle(std::uint8_t const *data, std::size_t const length) {
  auto const answer = [this](Command const command, CommandAck const status) {
    uint8_t commandAnswer[COMMAND_ANSWER_LENGTH] = {OTA_BLOCK_EMPTY_VALUE * COMMAND_ANSWER_LENGTH};

    commandAnswer[0] = COMMAND_ACK & 0xff;
    commandAnswer[1] = (COMMAND_ACK & 0xff00) >> 8;
    commandAnswer[2] = command & 0xff;
    commandAnswer[3] = (command & 0xff00) >> 8;
    commandAnswer[4] = status & 0xff;
    commandAnswer[5] = (status & 0xff00) >> 8;

    auto const crc16 = calculateCRC16(commandAnswer, 18);
    commandAnswer[18] = crc16 & 0xff;
    commandAnswer[19] = (crc16 & 0xff00) >> 8;

    m_commandCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);
  };

  if (length != 20) {
    ESP_LOGI(TAG, "recv ota start cmd wrong length");
    return;
  }

  auto const command = (data[0]) + (data[1] * 256);

  if (command == COMMAND_START) {
    m_currentSector = 0;
    m_currentPacket = 0;
    m_otaTotalLength = (data[2]) + (data[3] * 256) + (data[4] * 256 * 256) + (data[5] * 256 * 256 * 256);

    if (not m_updater->begin(m_otaTotalLength)) {
      answer(COMMAND_START, COMMAND_ACK_REFUSE);
      return;
    }

    ESP_LOGI(TAG, "recv ota start cmd, fw_length = %" PRIu32 "", m_otaTotalLength);

    resetBuffer();

    answer(COMMAND_START, COMMAND_ACK_ACCEPT);
  }

  if (command == COMMAND_END) {
    m_currentSector = 0;
    m_currentPacket = 0;
    m_otaTotalLength = 0;

    ESP_LOGI(TAG, "recv ota end cmd");

    m_updater->end();

    answer(COMMAND_END, COMMAND_ACK_ACCEPT);
  }
}

void MessageHandler::resetBuffer() {
  m_firmwareBufferOffset = 0;

  std::memset(m_firmwareBuffer, OTA_BLOCK_EMPTY_VALUE, OTA_BLOCK_SIZE);
}
