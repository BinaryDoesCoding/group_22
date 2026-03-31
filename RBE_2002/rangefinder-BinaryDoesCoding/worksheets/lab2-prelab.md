# Lab 2: Pre-lab Worksheet: Sensors and Filters

This is an individual assignment, but you may discuss the material with others, so long as the work you submit is your own.

Note that we assume you have taken a course in embedded systems, where you covered analog-to-digital conversion and interrupts.

## Interrupts, ADC, and Timers

> 1. Describe the main difference between an external interrupt and a pin change interrupt on the ATmega32U4. You might start with the paragraphs at the start of Chapter 11 in the datasheet and/or do a web search and/or dig through the `PCint` files in the utilities library.

> 2. A voltage of 2V is sampled by an 8-bit ADC that uses a reference voltage of 5 V. What is the output of the ADC? Take the floor and *be exact*.

> 3. What is the resolution (in mV/LSB) of a 12-bit ADC that uses a reference voltage of 3.3 V?

> 4. What method does the ATmega32U4 use for analog-to-digital conversion? What is the resolution if the reference voltage is 5V?

> 5. Consulting the ATmega32U4 datasheet, specifically the `ADCSRA` register on page 315, determine how long it takes to perform a single ADC conversion. You can find the ADC clock rate by dividing the system clock rate by the prescaler. The prescaler is found from `ADPS2:0`, which are set to `0b111` in the Arduino setup routines (in `wiring.c`, if you want to dig further). It takes one ADC clock cycle for each bit of the ADC, plus 3 extra cycles for setup/admin.

> 6. Write out a formula to convert an ADC reading on the ATmega32U4 to the *range* of voltages that could produce that result. Use a reference voltage of 5 V and assume the ADC takes the floor in the conversion.

> 7. Examine the `EventTimer` library. How does the library check if the timer has expired? Why not just check if the current time is greater than the end time?

## Ultrasonic Rangefinder

> 8. For the HC-SR04, what is the nominal conversion factor between the pulse width on the `ECHO` pin and distance? If a pulse is 10 ms long, how far away is the object?

> 9. For an object 40 cm away, how long is the pulse width?

> 10. What is the stated input range for the HC-SR04 ultrasonic sensor? What is the pulse width at the maximum range?

## Sharp IR Sensor

> 11. What is the nominal update interval for the Sharp IR sensor?

> 12. Using the nominal response for the Sharp IR proximity sensor (found in the datasheet), how far away is an object if the sensor produces an output of 1.5V?

## Filters

> 13. Before you get started with code, sketch out *functional architectures* for your filters, starting with inputting a new distance reading and ending with the filter output. Paste them here.

> 14. Paste your filter codes here (you can just copy/paste all of `filter.cpp`):
```
code block it
```

> 15. After you've passed the pre-build unit tests, review your architectures above and edit them for any differences. *Do not edit the previous version* -- you won't lose points! -- we want to see if you discovered anything that you hadn't considered in your original thinking. Paste your new architectures here.
