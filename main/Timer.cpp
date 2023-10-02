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

Timer::Timer() : m_enable(false), m_autoRepeat(true), m_delay(), m_startTime() {}

void Timer::setCallback(TimeoutCallbackFunction const& timeoutCallbackFunction)
{
    m_timeoutCallbackFunction = timeoutCallbackFunction;
}

void Timer::start(MicroSeconds const delay, bool const autoRepeat)
{
    m_delay = delay;
    m_startTime = std::chrono::system_clock::now();

    m_enable = true;
    m_autoRepeat = autoRepeat;
}

void Timer::stop()
{
    m_enable = false;
}

void Timer::process()
{
    if (not m_enable)
    {
        return;
    }

    if (not m_timeoutCallbackFunction)
    {
        return;
    }

    auto const currentTime = std::chrono::system_clock::now();
    auto const diffTime = std::chrono::duration_cast<MicroSeconds>(currentTime - m_startTime);
    if (diffTime < m_delay)
    {
        return;
    }

    m_timeoutCallbackFunction();

    if (not m_autoRepeat) {
        stop();
    }
}
