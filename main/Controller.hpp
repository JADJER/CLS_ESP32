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
// Created by jadjer on 09.02.23.
//


#pragma once

#include "configuration/IConfiguration.hpp"
#include "configuration/Configuration.hpp"

#include "pump/Pump.hpp"
#include "timer/Timer.hpp"
#include "button/Button.hpp"
#include "updater/Updater.hpp"
#include "distance/Distance.hpp"
#include "bluetooth/Bluetooth.hpp"
#include "power_manager/PowerManager.hpp"

#include <memory>

class Controller {
public:
    explicit Controller(IConfigurationPtr const& configuration);
    ~Controller();

public:
    [[noreturn]] void spin();

public:
    void spinOnce();

private:
    void sleep();
    void lubricateFromDistance();
    void lubricateFromTimer();
    void pumpStart();
    void pumpStop();
    void pumpManual();

private:
    IConfigurationPtr m_configuration;

private:
    std::unique_ptr<Pump> m_pump;
    std::unique_ptr<Timer> m_timer;
    std::unique_ptr<Button> m_button;
    std::unique_ptr<Updater> m_updater;
    std::unique_ptr<Distance> m_distance;
    std::unique_ptr<Bluetooth> m_bluetooth;
    std::unique_ptr<PowerManager> m_powerManager;
};
