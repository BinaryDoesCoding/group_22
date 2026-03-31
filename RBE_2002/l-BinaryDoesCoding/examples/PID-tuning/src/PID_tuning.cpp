#include <Arduino.h>
#include "chassis.h"
#include "serial_comm.h"

/**
 * For PI(D) tuning. Be sure to complete Romi32U4MotorBase::ControlMotorSpeed() before
 * starting the tuning.
 */

void setup(void)
{
    Serial.begin(115200);
    chassis.InititalizeChassis();
}

void loop(void)
{
    chassis.SpinOnce(); 
    if(CheckSerialInput()) ParseSerialInput();
}