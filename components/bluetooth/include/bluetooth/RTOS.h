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
// Created by jadjer on 16.02.23.
//

#pragma once

#include <freertos/FreeRTOS.h>// Include the base FreeRTOS definitions.
#include <freertos/semphr.h>  // Include the semaphore definitions.
#include <freertos/task.h>    // Include the task definitions.
#include <pthread.h>
#include <string>

/**
 * @brief Interface to %FreeRTOS functions.
 */
class FreeRTOS {
   public:
    static void sleep(uint32_t ms);
    static void startTask(void task(void*), std::string taskName, void* param = nullptr, uint32_t stackSize = 2048);
    static void deleteTask(TaskHandle_t pTask = nullptr);

    static uint32_t getTimeSinceStart();

    class Semaphore {
       public:
        Semaphore(std::string owner = "<Unknown>");
        ~Semaphore();
        void give();
        void give(uint32_t value);
        void giveFromISR();
        void setName(std::string name);
        bool take(std::string owner = "<Unknown>");
        bool take(uint32_t timeoutMs, std::string owner = "<Unknown>");
        std::string toString();
        uint32_t wait(std::string owner = "<Unknown>");
        bool timedWait(std::string owner = "<Unknown>", uint32_t timeoutMs = portMAX_DELAY);
        uint32_t value() { return m_value; };

       private:
        SemaphoreHandle_t m_semaphore;
        pthread_mutex_t m_pthread_mutex;
        std::string m_name;
        std::string m_owner;
        uint32_t m_value;
        bool m_usePthreads;
    };
};

/**
 * @brief Ringbuffer.
 */
// class Ringbuffer {
//    public:
// #ifdef ESP_IDF_VERSION_MAJOR
//     Ringbuffer(size_t length, RingbufferType_t type = RINGBUF_TYPE_NOSPLIT);
// #else
//     Ringbuffer(size_t length, ringbuf_type_t type = RINGBUF_TYPE_NOSPLIT);
// #endif
//     ~Ringbuffer();
//
//     void* receive(size_t* size, TickType_t wait = portMAX_DELAY);
//     void returnItem(void* item);
//     bool send(void* data, size_t length, TickType_t wait = portMAX_DELAY);
//
//    private:
//     RingbufHandle_t m_handle;
// };
