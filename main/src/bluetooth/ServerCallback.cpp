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

#include "bluetooth/ServerCallback.hpp"

void ServerCallback::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) {
  NimBLEServerCallbacks::onConnect(pServer, connInfo);
}
void ServerCallback::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, std::string &name) {
  NimBLEServerCallbacks::onConnect(pServer, connInfo, name);
}
void ServerCallback::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) {
  NimBLEServerCallbacks::onDisconnect(pServer, connInfo, reason);
}
void ServerCallback::onMTUChange(uint16_t MTU, NimBLEConnInfo &connInfo) {
  NimBLEServerCallbacks::onMTUChange(MTU, connInfo);
}
uint32_t ServerCallback::onPassKeyDisplay() {
  return NimBLEServerCallbacks::onPassKeyDisplay();
}
void ServerCallback::onConfirmPIN(NimBLEConnInfo &connInfo, uint32_t pin) {
  NimBLEServerCallbacks::onConfirmPIN(connInfo, pin);
}
void ServerCallback::onAuthenticationComplete(NimBLEConnInfo &connInfo) {
  NimBLEServerCallbacks::onAuthenticationComplete(connInfo);
}
void ServerCallback::onAuthenticationComplete(NimBLEConnInfo &connInfo, const std::string &name) {
  NimBLEServerCallbacks::onAuthenticationComplete(connInfo, name);
}
void ServerCallback::onIdentity(NimBLEConnInfo &connInfo) {
  NimBLEServerCallbacks::onIdentity(connInfo);
}
