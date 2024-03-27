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
// Created by jadjer on 01.10.23.
//

#include "Timer.hpp"

Timer::Timer() : m_enable(false), m_delay(), m_startTime() {}

bool Timer::isEnabled() const {
  return m_enable;
}

bool Timer::isCompleted() const {
  if (not m_enable) {
    return false;
  }

  auto const currentTime = std::chrono::system_clock::now();
  auto const diffTime = std::chrono::duration_cast<MicroSeconds>(currentTime - m_startTime);
  if (diffTime < m_delay) {
    return false;
  }

  return true;
}

void Timer::start(MicroSeconds const delay) {
  if (m_enable) {
    return;
  }

  m_delay = delay;
  m_startTime = std::chrono::system_clock::now();

  m_enable = true;
}

void Timer::stop() {
  m_enable = false;
}
