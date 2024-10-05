//
// Created by jadjer on 10/3/24.
//

#include "MessageHandler.hpp"

#include "utils.hpp"

#include <esp_log.h>

constexpr auto const TAG = "MessageHandler";

constexpr auto const OTA_BLOCK_SIZE = 4096;
constexpr auto const COMMAND_ANSWER_LENGTH = 20;
constexpr auto const COMMAND_START = 0x0001;
constexpr auto const COMMAND_END = 0x0002;
constexpr auto const COMMAND_ACK = 0x0003;
constexpr auto const COMMAND_ACK_ACCEPT = 0x0000;
constexpr auto const COMMAND_ACK_REFUSE = 0x0001;
constexpr auto const BINARY_ACK_SUCCESS = 0x0000;
constexpr auto const BINARY_ACK_CRC_ERROR = 0x0001;
constexpr auto const BINARY_ACK_SECTOR_INDEX_ERROR = 0x0002;
constexpr auto const BINARY_ACK_PAYLOAD_LENGTH_ERROR = 0x0003;

MessageHandler::MessageHandler(NimBLECharacteristic *dataCharacteristic, NimBLECharacteristic *commandCharacteristic) : m_updater(new Updater),
                                                                                                                        m_dataCharacteristic(dataCharacteristic),
                                                                                                                        m_commandCharacteristic(commandCharacteristic),
                                                                                                                        m_firmwareBuffer(new std::uint8_t[OTA_BLOCK_SIZE]) {
}

void MessageHandler::dataHandle(const std::uint8_t *data, std::size_t length) {
  uint8_t commandAnswer[COMMAND_ANSWER_LENGTH] = {0x03, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t crc16;

  auto const sector = (data[0]) + (data[1] * 256);

  if (sector != m_currentSector) {
    // sector error
    if (sector != 0xffff) {
      // sector error
      ESP_LOGE(TAG, "%s - sector index error, cur: %" PRIu32 ", recv: %d", __func__, m_currentSector, (data[0] + (data[1] * 256)));
      commandAnswer[0] = data[0];
      commandAnswer[1] = data[1];
      commandAnswer[2] = BINARY_ACK_SECTOR_INDEX_ERROR;
      commandAnswer[3] = 0x00;
      commandAnswer[4] = m_currentSector & 0xff;
      commandAnswer[5] = (m_currentSector & 0xff00) >> 8;
      crc16 = calculateCRC16(commandAnswer, 18);
      commandAnswer[18] = crc16 & 0xff;
      commandAnswer[19] = (crc16 & 0xff00) >> 8;

      m_dataCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);

      return;
    }
  }

  auto const packet = data[2];

  if (packet != m_currentPacket) {// packet seq error
    if (packet != 0xff) {         // last packet
      ESP_LOGE(TAG, "%s - packet index error, cur: %" PRIu32 ", recv: %d", __func__, m_currentPacket, packet);

      return;
    }
  }

  std::memcpy(m_firmwareBuffer + m_firmwareBufferOffset, data + 3, length - 3);
  m_firmwareBufferOffset += length - 3;

  if (packet == 0xff) {
    m_currentPacket = 0;
    m_currentSector++;

    goto sector_end;

  } else {
    m_currentPacket++;
  }

  return;

sector_end:
  if (m_firmwareBufferOffset < OTA_BLOCK_SIZE) {
    m_updater->writeData(m_firmwareBuffer, m_firmwareBufferOffset);
  } else {
    m_updater->writeData(m_firmwareBuffer, OTA_BLOCK_SIZE);
  }

  m_firmwareBufferOffset = 0;
  std::memset(m_firmwareBuffer, 0x0, OTA_BLOCK_SIZE);

  commandAnswer[0] = data[0];
  commandAnswer[1] = data[1];
  commandAnswer[2] = BINARY_ACK_SUCCESS;
  commandAnswer[3] = 0x00;
  crc16 = calculateCRC16(commandAnswer, 18);
  commandAnswer[18] = crc16 & 0xff;
  commandAnswer[19] = (crc16 & 0xff00) >> 8;

  m_dataCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);
}

void MessageHandler::commandHandle(const std::uint8_t *data, std::size_t length) {
  uint8_t commandAnswer[COMMAND_ANSWER_LENGTH] = {0x03, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t crc16;

  if (length < 2) {
    ESP_LOGI(TAG, "recv ota start cmd wrong length");
    return;
  }

  auto const command = (data[0]) + (data[1] * 256);

  if (command == COMMAND_START) {
    if (length < 6) {
      ESP_LOGI(TAG, "recv ota start cmd wrong length");
      return;
    }

    m_currentSector = 0;
    m_currentPacket = 0;
    m_otaTotalLength = (data[2]) + (data[3] * 256) + (data[4] * 256 * 256) + (data[5] * 256 * 256 * 256);

    ESP_LOGI(TAG, "recv ota start cmd, fw_length = %" PRIu32 "", m_otaTotalLength);

    m_updater->begin(m_otaTotalLength);

    std::memset(m_firmwareBuffer, 0x0, OTA_BLOCK_SIZE);

    commandAnswer[2] = 0x01;
    commandAnswer[3] = 0x00;
    crc16 = calculateCRC16(commandAnswer, 18);
    commandAnswer[18] = crc16 & 0xff;
    commandAnswer[19] = (crc16 & 0xff00) >> 8;

    m_commandCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);

    return;
  }

  if (command == COMMAND_END) {
    m_currentSector = 0;
    m_currentPacket = 0;
    m_otaTotalLength = 0;

    ESP_LOGI(TAG, "recv ota end cmd");

    m_updater->end();
    m_updater->install();

    commandAnswer[2] = 0x02;
    commandAnswer[3] = 0x00;
    crc16 = calculateCRC16(commandAnswer, 18);
    commandAnswer[18] = crc16 & 0xff;
    commandAnswer[19] = (crc16 & 0xff00) >> 8;

    m_commandCharacteristic->indicate(commandAnswer, COMMAND_ANSWER_LENGTH);
  }
}
