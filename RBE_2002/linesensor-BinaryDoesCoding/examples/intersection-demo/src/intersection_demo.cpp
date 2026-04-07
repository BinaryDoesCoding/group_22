#include <Arduino.h>
#include "line_sensor.h"

#define LIGHT_THRESHOLD 80
#define DARK_THRESHOLD  100

/**
 * This program simply prints the result of the intersection checker.
 * 
 * Useful for testing your hardware, once you have your algorithm working.
 * 
 * It does not drive the Romi, but you can make vroom, vroom sounds as you push it around
 * the arena
 */

PololuLineSensor lineSensor(A6, A2, LIGHT_THRESHOLD, DARK_THRESHOLD);

void setup(void)
{
    Serial.begin(115200);
    lineSensor.Init();
}

void loop(void)
{
    delay(200);
    Serial.println(lineSensor.CheckIntersection());
}