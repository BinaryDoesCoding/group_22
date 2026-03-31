#include "filter.h"

float BufferedFilter::AddReading(const float& newValue)
{
    /**
     * TODO: Add a reading to your circular buffer. 
     * 
     * WARNING: Be sure to wrap around when the index goes off the end!
     */

    // Store the new value at the current index
    buffer[index] = newValue;

    // Increment index and wrap around using modulo
    index = (index + 1) % SAMPLE_COUNT;

    /**
     * Leave this line here -- we return the new value for no particular reason,
     * but keep it consistent.
     */
    return newValue;
}

float MovingAverageFilter::CalcFiltered(const float& newValue)
{
    /**
     * TODO: Create your moving average filter. You'll first need to add
     * the value to the buffer (using the function above), then calculate
     * the average of all of the values in the buffer.
     */

    // Add new reading to circular buffer
    AddReading(newValue);

    // Compute sum of all values in buffer
    float sum = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        sum += buffer[i];
    }

    // Return average
    return sum / SAMPLE_COUNT;
}

float MedianFilter::CalcFiltered(const float& newValue)
{
    /**
     * TODO: Create your median filter. You'll first need to add
     * the value to the buffer (using the function above), then calculate
     * the mdeian of all of the values in the buffer.
     * 
     * WARNING: You can't sort in place, because then you won't be
     * overriding the right value!
     */

    // Add new reading to circular buffer
    AddReading(newValue);

    // Create a copy of the buffer so original order is preserved
    float temp[SAMPLE_COUNT];
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        temp[i] = buffer[i];
    }

    // Simple bubble sort (small array, so this is fine)
    for (int i = 0; i < SAMPLE_COUNT - 1; i++)
    {
        for (int j = 0; j < SAMPLE_COUNT - i - 1; j++)
        {
            if (temp[j] > temp[j + 1])
            {
                float swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }

    // Return middle value (since SAMPLE_COUNT = 9, middle index = 4)
    return temp[SAMPLE_COUNT / 2];
}


float LowPassFilter::CalcFiltered(const float& newValue)
{
    /**
     * TODO: Calculate the low-pass filter. For consistency, the new value
     * is multiplied by beta (not the carried average)
     */

    // Apply low-pass filter formula:
    // y[k] = beta * newValue + (1 - beta) * previous_output
    runningAvg = beta * newValue + (1 - beta) * runningAvg;

    return runningAvg;
}
