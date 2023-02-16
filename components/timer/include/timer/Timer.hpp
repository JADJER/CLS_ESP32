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
// Created by jadjer on 10.02.23.
//

#pragma once

#include <chrono>
#include <functional>

/**
 * @class Timer
 */
class Timer {
    using Callback = std::function<void()>;

   public:
    /**
     * Default constructor
     */
    Timer();
    /**
     * Default destructor
     */
    ~Timer();

   public:
    /**
     * Callback
     * @param callback
     */
    void setCompleteCallback(Callback const& callback);

   public:
    /**
     * Timer start
     * @param delay Timer disable after time
     */
    void start(std::chrono::seconds delay);
    /**
     * Timer stop
     */
    void stop();

   public:
    /**
     * Is timer enabled
     * @return True if enabled, otherwise false
     */
    [[nodiscard]] bool isEnabled() const;

   public:
    /**
     * @brief Timer control logic spin
     */
    void spinOnce();

   protected:
    bool m_isEnabled;
    Callback m_callback;
    std::chrono::seconds m_delay;
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
};
