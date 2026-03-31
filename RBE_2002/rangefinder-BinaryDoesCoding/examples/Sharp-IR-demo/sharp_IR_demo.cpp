#include "Sharp-IR.h"
#include <TeleplotUtils.h>

#define SHARP_IR_PIN A0

SharpIR proxSensor(SHARP_IR_PIN);

void setup(void)
{
    proxSensor.init();
}

void loop(void)
{
    float distance = 0;
    if(proxSensor.getDistance(distance)) TeleplotPrint("dist", distance);
}