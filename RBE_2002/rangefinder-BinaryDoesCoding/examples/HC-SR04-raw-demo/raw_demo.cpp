#include <HC-SR04.h>
#include <TeleplotUtils.h>

// examples/HC-SR04-filter-demo/raw_demo.cpp

#define ECHO 11
#define TRIG 4

HC_SR04 sr04(ECHO, TRIG);
void hcISR(void) {sr04.ISR_echo();}

void setup(void)
{
    sr04.init(hcISR);
}

void loop(void)
{
    float distance = 0;
    if(sr04.getDistance(distance)) TeleplotPrint("dist", distance);
}