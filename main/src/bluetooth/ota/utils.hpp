//
// Created by jadjer on 10/4/24.
//

#pragma once

#include <cstdint>

uint16_t calculateCRC16(std::uint8_t const *buf, std::size_t length) {
  uint16_t crc16 = 0;
  int32_t i;

  while (length--) {
    crc16 ^= *buf++ << 8;

    for (i = 0; i < 8; i++) {
      if (crc16 & 0x8000) {
        crc16 = (crc16 << 1) ^ 0x1021;
      } else {
        crc16 = crc16 << 1;
      }
    }
  }

  return crc16;
}
