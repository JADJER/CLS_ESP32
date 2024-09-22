// Copyright 2024 Pavel Suprunov
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
// Created by jadjer on 3/18/24.
//

#pragma once

#include <thread>

#include <esp_task_wdt.h>

#include "ExternalPower.hpp"
#include "Pump.hpp"
#include "Timer.hpp"
#include "WheelSensor.hpp"

#include "configuration/IConfiguration.hpp"

class Controller {
public:
  explicit Controller(IConfigurationPtr configuration);

public:
  [[noreturn]] void spin();

private:
  void spinOnce();

private:
  void pumpEnable();
  void pumpDisable();
  void sleep();

private:
  IConfigurationPtr m_configuration;

private:
  PumpPtr m_pumpPtr;
  TimerPtr m_timerPtr;
  WheelSensorPtr m_wheelSensorPtr;
  ExternalPowerPtr m_externalPowerPtr;

private:
  std::thread m_thread;
  esp_task_wdt_user_handle_t m_watchdogHandle;
};
