#include <Sharp-IR.h>
#include <TeleplotUtils.h>

/**
 * 500 is arbitrarily long -- check the data sheet for a reasonable value.
 * 
 * Alternatively, only return something when the value changes.
 */
const uint32_t SHARP_IR_INTERVAL = 50; 

bool SharpIR::getDistance(float& distance)
{
    bool newReading = false;
    uint32_t currTime = millis();
    if(currTime - lastReading >= SHARP_IR_INTERVAL)
    {
        lastReading = currTime;

        uint16_t adcResult = analogRead(adcPin);

        // TODO: Calculate the voltage
        float voltage = (adcResult / 1023.0) * 5.0;;

        /**
         * TODO: Convert to a distance.
         */
        //distance = voltage; // calculate from the voltage, once that is calculated

        // Convert voltage to distance (cm)
        TeleplotPrint("voltage", voltage);
    if(voltage > 0.1492)
    {
        distance = 21.063 / (voltage - 0.1492);
    }
    else
    {
        distance = 80; // max measurable distance 
    }

#ifdef __SHARP_DEBUG__
        Serial.print(adcResult);
        Serial.print('\t');

        Serial.print(voltage);
        Serial.print('\t');

        Serial.println(distance);
#endif

        newReading = true;
    }

    return newReading;
}
