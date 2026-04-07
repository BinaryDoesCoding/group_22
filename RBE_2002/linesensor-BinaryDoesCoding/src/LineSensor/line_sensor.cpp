#include "line_sensor.h"
// line_sensor.cpp - Line sensor class method definitions

bool LineSensor::CheckIntersection(const LineSensorReadings& readings)
{
    bool retVal = false;

    /**
     * Figure out if we're on tape. We'll start with true and then turn false if any
     * sensor is not on the tape.
     */
    bool onTape = true;

    /**
     * For now, basic intersection detection is:
     * both sensors see dark.
     *
     * You can add hysteresis later.
     */
    for (unsigned short i = 0; i < readings.elementCount; i++)
    {
        if (readings.adcReadings[i] < darkThreshold)
        {
            onTape = false;
        }
    }

    retVal = onTape;

    return retVal;
}

/**
 * This is just a helper function for using two sensors.
 */
bool LineSensor::CheckIntersection(int left, int right)
{
    LineSensorReadings readings(left, right);
    return LineSensor::CheckIntersection(readings);
}