#include "line_sensor.h"
#include <Arduino.h>

/**
 * WARNING: This is just a convenience function for a two-element array. If you
 * decide to use something more sophisticated, you'll need to comment it out and 
 * create a more general constructor!
 */
PololuLineSensor::PololuLineSensor(unsigned char leftPin, unsigned char rightPin, unsigned int light, unsigned int dark) : LineSensor(light, dark)
{
    pins[0] = leftPin;
    pins[1] = rightPin;
}

void PololuLineSensor::Init(void)
{
    /**
     * Set the pins to input (they default that way, but this is safer).
     */
    for(int i = 0; i < ELEMENT_COUNT; i++)
    {
        pinMode(pins[i], INPUT);
    }
}

bool PololuLineSensor::CheckIntersection(void)
{
    LineSensorReadings readings;
    for(int i = 0; i < ELEMENT_COUNT; i++)
    {
        readings.adcReadings[i] = analogRead(pins[i]);
    }

    return LineSensor::CheckIntersection(readings);
}

float PololuLineSensor::CalcError(void)
{
    /**
     * TODO: Calculate the error for line following
     */

     return 0;
}