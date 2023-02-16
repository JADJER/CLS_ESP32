// Copyright 2023 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 13.02.23.
//

#pragma once

#include <driver/gpio.h>

/**
 * @class Button
 */
class Button {
   public:
    /**
     * Default constructor
     */
    Button();
    /**
     * Default destructor
     */
    ~Button();

   public:
    /**
     * Check if button has been pressed
     * @return True if pressed, otherwise false
     */
    [[nodiscard]] bool isPressed() const;

   private:
    gpio_num_t m_buttonPin;
    uint8_t m_buttonPinEnableLevel;
};
