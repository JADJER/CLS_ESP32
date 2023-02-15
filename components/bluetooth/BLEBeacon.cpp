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

/*
 * BLEBeacon.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BLUEDROID_ENABLED)
#include <string.h>
#include "bluetooth/BLEBeacon.h"
#include "esp32-hal-log.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))


BLEBeacon::BLEBeacon() {
	m_beaconData.manufacturerId = 0x4c00;
	m_beaconData.subType        = 0x02;
	m_beaconData.subTypeLength  = 0x15;
	m_beaconData.major          = 0;
	m_beaconData.minor          = 0;
	m_beaconData.signalPower    = 0;
	memset(m_beaconData.proximityUUID, 0, sizeof(m_beaconData.proximityUUID));
} // BLEBeacon

std::string BLEBeacon::getData() {
	return std::string((char*) &m_beaconData, sizeof(m_beaconData));
} // getData

uint16_t BLEBeacon::getMajor() {
	return m_beaconData.major;
}

uint16_t BLEBeacon::getManufacturerId() {
	return m_beaconData.manufacturerId;
}

uint16_t BLEBeacon::getMinor() {
	return m_beaconData.minor;
}

BLEUUID BLEBeacon::getProximityUUID() {
	return BLEUUID(m_beaconData.proximityUUID, 16, false);
}

int8_t BLEBeacon::getSignalPower() {
	return m_beaconData.signalPower;
}

/**
 * Set the raw data for the beacon record.
 */
void BLEBeacon::setData(std::string data) {
	if (data.length() != sizeof(m_beaconData)) {
		log_e("Unable to set the data ... length passed in was %d and expected %d", data.length(), sizeof(m_beaconData));
		return;
	}
	memcpy(&m_beaconData, data.data(), sizeof(m_beaconData));
} // setData

void BLEBeacon::setMajor(uint16_t major) {
	m_beaconData.major = ENDIAN_CHANGE_U16(major);
} // setMajor

void BLEBeacon::setManufacturerId(uint16_t manufacturerId) {
	m_beaconData.manufacturerId = ENDIAN_CHANGE_U16(manufacturerId);
} // setManufacturerId

void BLEBeacon::setMinor(uint16_t minor) {
	m_beaconData.minor = ENDIAN_CHANGE_U16(minor);
} // setMinior

void BLEBeacon::setProximityUUID(BLEUUID uuid) {
	uuid = uuid.to128();
	memcpy(m_beaconData.proximityUUID, uuid.getNative()->uuid.uuid128, 16);
} // setProximityUUID

void BLEBeacon::setSignalPower(int8_t signalPower) {
	m_beaconData.signalPower = signalPower;
} // setSignalPower


#endif
