#pragma once
// line_sensor.h - Line sensor class definitions

#define ELEMENT_COUNT   2

struct LineSensorReadings
{
    unsigned short elementCount = ELEMENT_COUNT;
    unsigned int adcReadings[ELEMENT_COUNT];

    LineSensorReadings(void)
    {
        adcReadings[0] = 0;
        adcReadings[1] = 0;
    }

    LineSensorReadings(unsigned int l, unsigned int r)
    {
        adcReadings[0] = l;
        adcReadings[1] = r;
    }
};

class LineSensor
{
protected:
    /**
     * Thresholds for light and dark (hysteresis). To start, just use the dark.
     *
     * These get set in the constructor.
     */
    unsigned int lightThreshold;
    unsigned int darkThreshold;

public:
    LineSensor(unsigned int light, unsigned int dark)
    {
        lightThreshold = light;
        darkThreshold = dark;
    }

    bool CheckIntersection(const LineSensorReadings&);
    bool CheckIntersection(int, int);
};

class PololuLineSensor : public LineSensor
{
protected:
    /**
     * Pin numbers (use A0, A2, etc.) for reading sensors.
     */
    unsigned char pins[ELEMENT_COUNT];

public:
    PololuLineSensor(unsigned char leftPin, unsigned char rightPin, unsigned int light, unsigned int dark);

    void Init(void);

    bool CheckIntersection(void);
    float CalcError(void);
};