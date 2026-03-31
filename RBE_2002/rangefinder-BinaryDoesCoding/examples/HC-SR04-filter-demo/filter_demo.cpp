#include "HC-SR04.h"
#include <TeleplotUtils.h>
#include <filter.h>

// examples/HC-SR04-filter-demo/filter_demo.cpp

#define ECHO 11
#define TRIG 4

HC_SR04 sr04(ECHO, TRIG);
void hcISR(void) {sr04.ISR_echo();}

LowPassFilter lowPass(0.1);
MedianFilter median;
MovingAverageFilter moving;

void setup(void)
{
    sr04.init(hcISR);
}

void loop(void)
{
    float distance = 0;
    if(sr04.getDistance(distance)) 
    {
        TeleplotPrint("raw", distance);
        TeleplotPrint("lowpass", lowPass.CalcFiltered(distance));
        TeleplotPrint("moving", moving.CalcFiltered(distance));
        TeleplotPrint("median", median.CalcFiltered(distance));
    }
}