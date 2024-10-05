//
// Created by jadjer on 10/2/24.
//

#include "UpdateCharacteristicCallback.hpp"

#include "bluetooth/Identificator.hpp"

UpdateCharacteristicCallback::UpdateCharacteristicCallback(MessageHandlerPtr messageHandler) : m_messageHandler(std::move(messageHandler)) {
}

void UpdateCharacteristicCallback::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
  NimBLECharacteristicCallbacks::onRead(pCharacteristic, connInfo);
}

void UpdateCharacteristicCallback::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
  NimBLECharacteristicCallbacks::onWrite(pCharacteristic, connInfo);

  NimBLEUUID const uuid = pCharacteristic->getUUID();

  if (uuid == CHARACTERISTIC_DATA_UUID) {
    auto const value = pCharacteristic->getValue();

    auto const length = value.length();
    auto const data = value.data();

    m_messageHandler->dataHandle(data, length);
  }

  if (uuid == CHARACTERISTIC_COMMAND_UUID) {
    auto const value = pCharacteristic->getValue();

    auto const length = value.length();
    auto const data = value.data();

    m_messageHandler->commandHandle(data, length);
  }
}

void UpdateCharacteristicCallback::onStatus(NimBLECharacteristic *pCharacteristic, int code) {
  NimBLECharacteristicCallbacks::onStatus(pCharacteristic, code);
}

void UpdateCharacteristicCallback::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {
  NimBLECharacteristicCallbacks::onSubscribe(pCharacteristic, connInfo, subValue);
}
