#pragma once

/**
 * A hierarchy of Filter classes. Filter is a virtual class -- you can't declare an
 * object of type Filter. Any derived class that you wish to instantiate _must_ define
 * a CalcFiltered() method.
 * 
 * Note that we have in "intermediate" class to manage a circular buffer, but it doesn't
 * define CalcFiltered(), either.
 */

class Filter
{
public:
    // This declaration makes Filter an abstract class -- you cannot declare
    // an object of type Filter. You have to use the derived classes.
    virtual float CalcFiltered(const float&) = 0;
};

class LowPassFilter : public Filter
{
protected:
    float beta = 0;
    float runningAvg = 0;

public:
    LowPassFilter(float b) { beta = b; }
    virtual float CalcFiltered(const float&);
};

/**
 * For the buffered filters, we hard-code the sample count to make declaring 
 * arrays easier (the standard templates like vector don't always play nice \
 * with microcontrollers).
 * 
 * Formally, the median for an even number of values is the average of the two in
 * the middle. For the basic work, just keep the count odd and take the middle value.
 * 
 * You can manage even counts as an extra.
 */
const unsigned char SAMPLE_COUNT = 9;

class BufferedFilter : public Filter
{
protected:
    unsigned char index = 0;
    float buffer[SAMPLE_COUNT];
    BufferedFilter(void)
    {
        for (unsigned char i = 0; i < SAMPLE_COUNT; i++)
        {
            buffer[i] = 0;
        }
    }

public:
    float AddReading(const float&);
};

class MedianFilter : public BufferedFilter
{
public:
    float CalcFiltered(const float&);
};

class MovingAverageFilter : public BufferedFilter
{
public:
    float CalcFiltered(const float& newValue);
};
