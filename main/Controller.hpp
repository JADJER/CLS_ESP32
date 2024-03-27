//
// Created by jadjer on 3/18/24.
//

#pragma once

#include "Pump.hpp"
#include "Timer.hpp"
#include "SpeedSensor.hpp"
#include "ExternalPower.hpp"

#include "executor/Node.hpp"

class Controller : public executor::Node {
public:
  Controller();
  ~Controller() override = default;

private:
  void process() override;

private:
  void sleep();

private:
  PumpPtr m_pumpPtr;
  TimerPtr m_timerPtr;
  SpeedSensorPtr m_speedSensorPtr;
  ExternalPowerPtr m_externalPowerPtr;
};
