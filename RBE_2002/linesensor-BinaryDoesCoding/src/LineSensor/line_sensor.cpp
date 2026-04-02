#include <line_sensor.h>

bool LineSensor::CheckIntersection(const LineSensorReadings& readings)
{
    bool retVal = false;

    /**
     * Figure out if we're on tape. We'll start with true and then turn false if any
     * sensor is not on the tape.
     */
    bool onTape = true;

    /**
     * TODO: Add hysteresis (after you get the basic event checking working).
     */
    for(unsigned short i = 0; i < readings.elementCount; i++)
    {
        if(readings.adcReadings[i] < darkThreshold) onTape = false;
    }

    /**
     * This looks silly, but that's because the logic is wrong. It will be useful
     * to keep track of whether or not the sensors all see tape, so we leave the
     * skeleton here.
     * 
     * TODO: Create the proper event-checker logic.
     */
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
