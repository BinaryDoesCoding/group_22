# Lab 2: Group Worksheet

## Ultrasonic Rangefinder

### Preliminaries

> 1. Explain how new distance readings are retrieved from sensor. Why does `getDistance()` return a `bool`?

The HC-SR04 library is interrupt-driven. A ping is sent from the trigger pin, and the echo pin is monitored by an ISR. The ISR records the start time when the echo pin goes high and the end time when the echo pin goes low. The `getDistance()` function checks whether a full echo pulse has been received yet. It returns a `bool` so the caller knows whether the reported value is actually a new valid reading or whether no complete reading is ready yet.

> 2. Explain the use of the bitwise operator in the following code. What does the line of code do?

This uses a bitwise AND to test whether the `ECHO_RECD` bit is set inside the `state` variable. In plain English, the line asks whether the sensor has finished receiving an echo pulse. If that flag is present, the code knows a pulse length is ready to be processed.
    
```    
    if(state & ECHO_RECD)
```

> 3. Explain how the ISR is used to capture the pulse length for the HC_SR04.

The ISR watches the echo pin for changes. When the echo pin rises, the ISR stores the start time. When the echo pin falls, the ISR stores the end time and marks that the echo has been received. The pulse length is then found by subtracting the start time from the end time.

> 4. How is the state of the sensor stored? How does the library set the states?

The library stores sensor status in a state variable that uses bit flags. Different bits represent conditions such as idle, ping sent, echo currently high, echo received, or error. The code sets or combines these states with bitwise operations as the measurement progresses.

### Performance

> 5. What is the measured pulse width (on the oscilloscope) when an object is 40cm away? How does that compare to the expected value?

The measured pulse width of an object 40cm away was 2360μs, the expected pulse width for an object 40cm away is 2320μs (error= 40μs or 1.72%)

| Distance | Pulse width (osc.) | Pulse width (code) | Pulse width (expected)|
|:--:|:--:|:--:|:--:|
| 40 cm | 2360μs | 2350μs | 2320μs |

> 6. When the sensor times out, how long is the echo pulse? How does that compare to pulse at the maximum range?

When the sensor times out, the echo pulse is ~70650μs, about 3 times the pulse at the maximum range of the HC-SR04 (~23200μs).

> 7. What did you do to filter out the excessively long pulses (time outs)?

We added a check in `HC_SR04::getDistance()` to ignore echo pulses that are longer than the maximum valid sensor range instead of converting them into a false distance reading. In other words, if the pulse width is greater than the maximum allowed value, the code rejects it.

> 8. For each of the three filters, list the key parameters that you settled on. Explain why you chose them. What are the trade-offs of making the parameters larger or smaller?

- **Moving average filter**  
  Key parameter: window size `N`  
  Larger `N` gives smoother output but slower response. Smaller `N` reacts faster but passes more noise.

- **Median filter**  
  Key parameter: window size `N`  
  Larger `N` rejects outliers more strongly but responds more slowly. Smaller `N` is faster but less robust to spikes.

- **Low-pass filter**  
  Key parameter: `beta`  
  Larger `beta` trusts the new reading more and responds faster, but it is noisier. Smaller `beta` makes the output smoother, but the response is slower.

> 9. Paste the step response plot(s) here. You can consolidate code and print one graph or paste three separate graphs, but each needs both raw and filtered. Compare the performance of the three filters. What are the advantages/disadvantages of each?

![](Figures/Low_pass_step_response.png)
![](Figures/Moving_average_step_response.png)
![](Figures/Median_filter_step_response.png)

Low-pass filter advantages:

Low-pass filter disadvantages:

Moving average filter advantages:

Moving average filter disadvantages:

Median filter advantages:

Median filter disadvantages:



> 10. Record the average pulse width for each of the distances below. Comment on the agreement.

| Distance | Pulse width (moving average) | Pulse width (expected)|
|:--:|:--:|:--:|
| 40 cm | 2460 μs| 2320 μs|
| 80 cm | 4730 μs| 4640 μs|
| 120 cm | 5408 μs| 6960 μs|



> **11. Record your transfer function here.**
>
> $$\Delta t = f(d)$$

The idea is simple. As the object gets farther away, the sound wave takes longer to travel out and back, so the pulse width gets bigger. The lab says to model that with a straight line, so we use

$$\Delta t = ad + b$$

where:
- \(d\) is distance in cm
- \(\Delta t\) is pulse width in \(\mu s\)

Using the measured points:

| Distance | Pulse width (moving average) | Pulse width (expected)|
|:--:|:--:|:--:|
| 40 cm | 2460 μs| 2320 μs|
| 80 cm | 4730 μs| 4640 μs|
| 120 cm | 5408 μs| 6960 μs|

the best-fit line is

$$\boxed{\Delta t = 36.85d + 1251.33}$$

This means the pulse width increases by about **36.85 μs** for every extra centimeter. The \(1251.33\) is the offset from the line fit.

> 12. Record the reported distance for each of the distances below. Comment on the agreement.

| Distance | Reported distance |
|:--:|:--:|
| 40 cm |  39.72 cm|
| 100 cm | 92.75 cm |

The ultrasonic sensor can be affected by various factors. A common issue, as explained in Section 13, is the sensor's dead zone at close distances. You can refer to that section for a clearer explanation. At close range, the sensor's readings may be inconsistent, but as the distance increases, the accuracy improves. This inconsistency often occurs because the sensor is not operating in an ideal environment. Common factors include:



- Soft surfaces: Absorb sound, preventing reflection

- Angled surfaces: Deflect the echo away from the sensor

- Air turbulence: Scatters sound waves, reducing accuracy

- Dust or dirt: Obstructs the transducer, causing erratic readings



Ideally, to achieve a better transfer function and potentially increase accuracy, we would add more distance points and collect more pulse widths. 

> 13. What is the shortest distance you can reliably measure? What happens when the object gets too close to the sensor?

The shortest distance that can be reliably measured is about 2 cm.

When the object gets too close, it enters the sensor’s dead zone or blind zone. In this region, the sensor cannot reliably detect the reflected echo because the transducer is still ringing after sending the ultrasonic pulse. As a result, the sensor may return inaccurate values, unstable readings, or no valid reading at all.

## Sharp IR Proximity Sensor

### Preliminaries

> 14. Explain how new distance readings are retrieved from sensor. Why does `getDistance()` return a `bool`?

The getDistance() function retrieves new distance readings by using a timing-based approach (as opposed to reading the sensor every time it is called). Each time the function runs, it checks the current time using millis() and compares it to the time of the last reading; only if the SHARP_IR_INTERVAL has passed does it proceed to take a new measurement. When this condition is met, it updates the lastReading timestamp and calls analogRead(adcPin) to obtain a raw ADC value corresponding to the sensor’s output voltage. This value is intended to be converted into a voltage and then into a distance.`

`The function returns a bool to indicate whether a new reading was actually taken. It returns true if a fresh measurement was made during that call, and false if not enough time has passed and no new data is available.

> 15. How is the timing of the readings managed?

The timing of the readings is managed using a non-blocking timer based on millis(), which tracks how much time has passed since the program started. Each time getDistance() is called, it reads the current time (currTime) and compares it to the time of the last successful reading (lastReading). If the difference is greater than or equal to the defined interval (SHARP_IR_INTERVAL), the function takes a new sensor reading and updates lastReading to the current time. If not enough time has passed, the function skips the reading.

### Transfer function

> 16. Record the output voltage of the sensor for each distance.

| Distance | Voltage | 
|:--:|:--:|
| 5 cm | 3 v |
| 10 cm | 2.25 v |
| 15 cm | 1.57 v |
| 20 cm | 1.2 v |
| 40 cm | 0.65 v |
| 60 cm | 0.50 v |
| 80 cm | 0.43 v |
| 100 cm | 0.29 v |

> 17. Include two plots: one with the distance ($x$) on the horizontal axis and voltage on the vertical and then another with $1/x$ on the horizontal axis and your best fit line overlayed on it. Comment on the agreement with the datasheet. Did you use all of the datapoints for finding the best-fit line?

![](Figures/Chart2.png)
![](Figures/Chart1.png)

The plots do line up relatively closely with the ones shown in the datasheet.

Also, we did not use all of the datapoints for finding the best-fit line. We specifically excluded the 5 cm and 100 cm distances, as they are outside the IR measuring range of 10 cm to 80 cm (as seen in the datasheet).

> 18. Write an expression for the gain of the proximity sensor, $dV/dx$. As an object moves farther from the sensor, does the precision improve or get worse? Explain in terms of the mathematical expression you derived.

Based on our line of best fit, our expression is V = 21.063x+ 0.1492 where x = 1/d.

As an object moves fartherer from the sensor, the precision will get worse. This is becuase, as d (distance) increases, it will produce smaller changes in x. This in turn, will led to smaller changes in voltage at larger distances, making it harder to distinguish between distances based on the sensor output.

> 19. Complete the table below after you've edited the code to return a distance. Comment on the agreement.

| Actual distance | Voltage | Reported distance |
|:--:|:--:|:--:|
| 5 cm | 3 v | 7.3 cm |
| 20 cm | 1.2 v | 20.87 cm |
| 50 cm | 0.54 v | 51.02 cm |

> 20. Why is the reported distance at 5 cm so poor?

The reported distance at 5 cm is so poor becuase that distance is outside the 10 cm to 80 cm measurable range of the IR sensor.

> 21. Paste the contents of `SharpIR::getDistance()` here.

```
#include <sharp-IR.h>
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

```
## Bang-bang Control

> 22. Comment on the performance.

> 23. Paste your event checkers and handlers here.

```
Use a code block.
```

## Alignment with Proportional Control

> 24. Submit a $clickable$ link to a video of one of robots aligning with a garbage can.

> 25. Paste your distance handler here. 

```
Use a code block.
```

## Wrapping up

> 26. Submit a clickable link to release of each student's Robot repository (be sure to choose the `lab02` branch). Tag and title the release `lab02-last-first`. When done, you can merge the branch in to `main`.

> 27. Use your experience from this lab and do some research to identify some of the strengths and limitations of an ultrasonic rangefinder, whether the specific model in this lab or ultrasonic sensors, in general. List at least three strengths and three weaknesses.

> 28. What are some of the advantages and disadvantages of the IR proximity sensor? List at least three of each.

### Advantages
1. It updates quickly and is simple to interface with an analog pin.
2. It works well for short-range sensing.
3. It does not depend on sound reflections.

### Disadvantages
1. Its transfer function is nonlinear and must be calibrated.
2. Its useful range is limited.
3. It can be affected by target surface properties and sensor geometry.
