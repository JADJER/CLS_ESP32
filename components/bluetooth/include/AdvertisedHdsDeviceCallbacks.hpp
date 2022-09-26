//
// Created by jadjer on 24.09.22.
//

#pragma once

#include <BLEAdvertisedDevice.h>

class AdvertisedHDSDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
 public:
  void onResult(BLEAdvertisedDevice advertisedDevice) override;
};
