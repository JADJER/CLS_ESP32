//
// Created by jadjer on 01.10.23.
//

#include "DistanceSensor.hpp"

#include "gpio/InputPin.hpp"

constexpr float wheelLength_InMeters = 1.2;
constexpr float wheelLength_InKilometers = wheelLength_InMeters / 1000;

DistanceSensor::DistanceSensor(uint8_t const numberOfPin, PinState const defaultLevel) :
    m_distance(0), m_distanceSensorState(), m_distanceSensorPin(std::make_unique<gpio::InputPin>(numberOfPin, defaultLevel))
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
