//
// Created by jadjer on 01.10.23.
//

#include "DistanceSensor.hpp"

#include "gpio/Pin.hpp"

constexpr float wheelLength_InMeters = 1.2;
constexpr float wheelLength_InKilometers = wheelLength_InMeters / 1000;

DistanceSensor::DistanceSensor() :
    m_distance(0), m_distanceSensorPin(std::make_unique<gpio::Pin>(18, gpio::PIN_LEVEL_LOW)), m_distanceSensorState()
{
}

void DistanceSensor::setCallback(DistanceSensorCallbackFunction const& callback)
{
    m_callback = callback;
}

void DistanceSensor::process()
{
    if (not m_callback)
    {
        return;
    }

    auto const distanceSensorLevel = m_distanceSensorPin->getLevel();
    if (m_distanceSensorState != distanceSensorLevel)
    {
        if (distanceSensorLevel == gpio::PIN_LEVEL_HIGH)
        {
            m_distance += wheelLength_InKilometers;
        }

        m_distanceSensorState = distanceSensorLevel;
    }

    m_callback(m_distance);
}
