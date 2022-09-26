//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>

Controller::Controller() : m_indicator(2), m_bluetooth("CLS") {
  log_i("Start bluetooth server...");
  m_indicator.blink(100);

  m_bluetooth.createService(SERVICE_STATE_UUID, {
                                                    STATE_CONNECTED_UUID,
                                                });

  m_bluetooth.start();
  m_bluetooth.setValueString(STATE_CONNECTED_UUID, "Connecting...");
  m_bluetooth.advertising();

  log_i("Connect to ECU...");
  m_indicator.blink(250);


  m_bluetooth.setValueString(STATE_CONNECTED_UUID, "Connected");

  log_i("Detect active tables...");
  m_indicator.blink(500);

  m_ecu.detectActiveTables();

  log_i("Initialize done");
  m_indicator.enable();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(10);
  }
}

void Controller::spinOnce() {

  auto vehicleData = m_ecu.getVehicleData();
  m_bluetooth.setValueString(VEHICLE_BATTERY_UUID, std::to_string(vehicleData.batteryVolts));
  m_bluetooth.setValueString(VEHICLE_SPEED_UUID, std::to_string(vehicleData.speed));
  m_bluetooth.setValueString(VEHICLE_STATE_UUID, std::to_string(vehicleData.state));

  auto engineData = m_ecu.getEngineData();
  m_bluetooth.setValueString(ENGINE_RPM_UUID, std::to_string(engineData.rpm));
  m_bluetooth.setValueString(ENGINE_FUEL_INJECT_UUID, std::to_string(engineData.fuelInject));
  m_bluetooth.setValueString(ENGINE_IGNITION_ADVANCE_UUID, std::to_string(engineData.ignitionAdvance));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_1_UUID, std::to_string(engineData.unkData1));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_2_UUID, std::to_string(engineData.unkData2));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_3_UUID, std::to_string(engineData.unkData3));

  auto sensorsData = m_ecu.getSensorsData();
  m_bluetooth.setValueString(SENSORS_TPS_PERCENT_UUID, std::to_string(sensorsData.tpsPercent));
  m_bluetooth.setValueString(SENSORS_TPS_VOLTAGE_UUID, std::to_string(sensorsData.tpsVolts));
  m_bluetooth.setValueString(SENSORS_IAT_TEMP_UUID, std::to_string(sensorsData.iatTemp));
  m_bluetooth.setValueString(SENSORS_IAT_VOLTAGE_UUID, std::to_string(sensorsData.iatVolts));
  m_bluetooth.setValueString(SENSORS_ECT_TEMP_UUID, std::to_string(sensorsData.ectTemp));
  m_bluetooth.setValueString(SENSORS_ECT_VOLTAGE_UUID, std::to_string(sensorsData.ectVolts));
  m_bluetooth.setValueString(SENSORS_MAP_PRESSURE_UUID, std::to_string(sensorsData.mapPressure));
  m_bluetooth.setValueString(SENSORS_MAP_VOLTAGE_UUID, std::to_string(sensorsData.mapVolts));
}
