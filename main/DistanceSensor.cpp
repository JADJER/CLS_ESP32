//
// Created by jadjer on 01.10.23.
//

#include "DistanceSensor.hpp"

#include "gpio/InputPin.hpp"

constexpr const uint8_t numberOfDistanceSensorPin = 18;

constexpr float const wheelLength_InMeters = 1.2;
constexpr float const wheelLength_InKilometers = wheelLength_InMeters / 1000;

DistanceSensor::DistanceSensor() :
    m_distance(0), m_distanceSensorState(gpio::PIN_LEVEL_UNKNOWN),
    m_distanceSensorPin(std::make_unique<gpio::InputPin>(numberOfDistanceSensorPin, gpio::PIN_LEVEL_HIGH))
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
    if (distanceSensorLevel == m_distanceSensorState)
    {
        return;
    }

    m_distanceSensorState = distanceSensorLevel;

    if (distanceSensorLevel == gpio::PIN_LEVEL_HIGH)
    {
        return;
    }

    m_distance += wheelLength_InKilometers;

    m_callback(m_distance);
}
