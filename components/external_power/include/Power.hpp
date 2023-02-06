//
// Created by jadjer on 30.08.22.
//

#pragma once

#include <Arduino.h>

/**
 * @brief
 */
class Power {
 public:
  explicit Power(int8_t buttonPin);
  ~Power();

 public:
  /**
   * @brief
   * @return
   */
  [[nodiscard]] bool isEnabled() const;

 public:
  void sleep();

 private:
  int8_t m_powerPin;
};
