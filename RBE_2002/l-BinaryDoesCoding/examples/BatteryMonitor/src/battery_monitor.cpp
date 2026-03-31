#include <Arduino.h>
#include "chassis.h"

Chassis chassis;

/**
 * This will allow you to test your battery monitor. We'll leave it to you to devise
 * the physical test for your monitor.
 * 
 * WARNING: This example engages the Chassis/Motor machinery (needed to flash pin 13).
 * It shouldn't drive anywhere, but best to hold it in your hand, just in case!
 */

void setup(void)
{
    Serial.begin(115200);
    chassis.InititalizeChassis();
}

void loop(void)
{
    chassis.SpinOnce();   
}