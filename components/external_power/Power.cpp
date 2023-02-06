//
// Created by jadjer on 30.08.22.
//

#include "Power.hpp"
#include <driver/rtc_io.h>

Power::Power(int8_t buttonPin) {
  m_powerPin = buttonPin;
  pinMode(m_powerPin, INPUT_PULLUP);
  rtc_gpio_pullup_en((gpio_num_t) m_powerPin);
}

Power::~Power() = default;

bool Power::isEnabled() const {
  auto isPowered = digitalRead(m_powerPin);
  return not isPowered;
}

void Power::sleep() {
  esp_sleep_enable_ext0_wakeup((gpio_num_t) m_powerPin, LOW);
  log_i("Sleep");
  esp_deep_sleep_start();
}
