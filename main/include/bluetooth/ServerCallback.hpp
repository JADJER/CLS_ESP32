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
// Created by jadjer on 9/25/24.
//

#pragma once

#include "NimBLEServer.h"

class ServerCallback : public NimBLEServerCallbacks {
public:
  ~ServerCallback() override = default;

public:
  void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override;
  void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, std::string &name) override;
  void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override;
  void onMTUChange(uint16_t MTU, NimBLEConnInfo &connInfo) override;
  uint32_t onPassKeyDisplay() override;
  void onConfirmPIN(NimBLEConnInfo &connInfo, uint32_t pin) override;
  void onAuthenticationComplete(NimBLEConnInfo &connInfo) override;
  void onAuthenticationComplete(NimBLEConnInfo &connInfo, const std::string &name) override;
  void onIdentity(NimBLEConnInfo &connInfo) override;
};
