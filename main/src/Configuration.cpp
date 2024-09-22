//
// Created by jadjer on 9/20/24.
//

#include "configuration/Configuration.hpp"

constexpr uint64_t const HOUR = 60;
constexpr uint64_t const MINUTE = 60;
constexpr uint64_t const KILOMETER = 1000;

uint8_t Configuration::getExternalPowerPin() const {
  return 15;
}

uint8_t Configuration::getPumpPin() const {
  return 16;
}

uint8_t Configuration::getWheelSensorPin() const {
  return 14;
}

uint64_t Configuration::getTimeForEnable() const {
//  return 5 * MINUTE * HOUR;
  return 15;
}

uint64_t Configuration::getDistanceForEnable() const {
  return 500 * KILOMETER;
}

uint64_t Configuration::getDelayAfterBoot() const {
  return 10 * MINUTE;
}

uint64_t Configuration::getPumpTimout() const {
  return 60;
}

uint64_t Configuration::getWheelLength() const {
  return 1300;
}
