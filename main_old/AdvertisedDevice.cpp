// Copyright 2022 Pavel Suprunov
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
// Created by jadjer on 27.09.22.
//

#include "AdvertisedDevice.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>
#include <BLEDevice.h>

AdvertisedDevice::AdvertisedDevice() {
  m_device = nullptr;
}

void AdvertisedDevice::onResult(BLEAdvertisedDevice advertisedDevice) {
  if (not advertisedDevice.haveServiceUUID()) {
    return;
  }

  if (not advertisedDevice.isAdvertisingService(serviceAdvertiseUUID)) {
    return;
  }

  BLEDevice::getScan()->stop();

  m_device = new BLEAdvertisedDevice(advertisedDevice);
}

BLEAdvertisedDevice* AdvertisedDevice::getAdvertisedDevice() const {
  return m_device;
}
