#include <Arduino.h>
#include "line_sensor.h"

#define LIGHT_THRESHOLD 80
#define DARK_THRESHOLD  100

/**
 * This program prints the error from the line sensor. Useful for checking if your
 * calculations result in a CW or CCW turn.
 */

PololuLineSensor lineSensor(A0, A4, LIGHT_THRESHOLD, DARK_THRESHOLD);

void setup(void)
{
    Serial.begin(115200);
}

void loop(void)
{
    delay(500);
    Serial.println(lineSensor.CalcError());
}