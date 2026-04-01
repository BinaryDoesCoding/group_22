#include <HC-SR04.h>
#include <PCint.h>

#define IDLE        0x00
#define PING_SENT   0x01
#define ECHO_HIGH   0x02
#define ECHO_RECD   0x04
#define ERROR       0x08

/** \brief Constructor.
 * 
 * @param echo The echo pin. Must be interrupt enabled. PCInts OK.
 * @param trig The trigger pin.
 * */
HC_SR04::HC_SR04(uint8_t echo, uint8_t trig) : Rangefinder()
{
    echoPin = echo;
    trigPin = trig;
}

// sets up the interface
void HC_SR04::init(void (*isr)(void))
{
  if(!isr)
  {
    Serial.println("No ISR defined!");
    return;
  }

  // ensure ECHO pin is an input
  pinMode(echoPin, INPUT);

  // register the interrupt for the echo
  if(digitalPinToInterrupt(echoPin) != NOT_AN_INTERRUPT)
  {
    Serial.println("Attaching rangefinder ISR");
    attachInterrupt(digitalPinToInterrupt(echoPin), isr, CHANGE);
  }
  else if(digitalPinToPCInterrupt(echoPin) != NOT_AN_INTERRUPT)
  {
    Serial.println("Attaching rangefinder PC_ISR");
    attachPCInt(digitalPinToPCInterrupt(echoPin), isr);
  }
  else
  {
    while(true)
    {
      // We intentionally block here -- need to find a different pin
      Serial.println("Not a rangefinder interrupt pin!");
      delay(100);
    }
  }

  //control pin for commanding pings must be an output
  pinMode(trigPin, OUTPUT);
}

/**
 * \brief commandPing() sends a ping. 
 * It first checks to see if the device is IDLE to avoid interference.
 * 
 * Returns true if a ping is sent, in case you want to manually send pings and check
 */
bool HC_SR04::commandPing(void)
{
    bool pingSent = false;
    uint32_t currTime = millis();

    if (state == IDLE)
    {
        if (currTime - lastPingCheck >= 60)
        {
            lastPingCheck = currTime;

            cli();
            pulseEnd = 0;
            pulseStart = 0;
            state = PING_SENT;
            sei();

            digitalWrite(trigPin, HIGH);
            delayMicroseconds(12);
            digitalWrite(trigPin, LOW);

            pingSent = true;
        }
    }
    else
    {
        lastPingCheck = currTime;
    }

    return pingSent;
}

bool HC_SR04::getDistance(float& distance)
{
    bool retVal = false;

    if (state == ERROR)
    {
        lastPingCheck = millis();
        state = IDLE;
    }

    uint32_t echoLength = 0;

    if (state & ECHO_RECD)
    {
        cli();
        echoLength = (pulseEnd - pulseStart);
        state = IDLE;
        sei();

        lastPingCheck = millis();

        // Reject echoes that are too short or too long
        // Rough usable bounds for HC-SR04
        if (echoLength >= 150 && echoLength <= 25000)
        {
            distance = (echoLength - 1251.33f) / 36.85f;

            // Extra sanity clamp
            if (distance >= 0.0f && distance <= 400.0f)
            {
                retVal = true;
            }
        }
    }

    commandPing();

    return retVal;
}

/** \brief ISR for the echo pin
 * 
 * Records both the start and stop (rise and fall) of the echo pin.
 * When the pin goes low, it sets a flag.
 * */
void HC_SR04::ISR_echo(void)
{
  if(digitalRead(echoPin))      // transitioned to HIGH
  {
    if(state & PING_SENT)
    {
        pulseStart = micros();
        state |= ECHO_HIGH;
    }
    else                        // something went wrong
    {
      state = ERROR;
    }
  }

  if(!digitalRead(echoPin))     // transitioned to LOW
  {
    if(state & ECHO_HIGH)
    {
        pulseEnd = micros();
        state |= ECHO_RECD;
    } 
    else                        // something went wrong
    {
      state = ERROR;
    }
  }
}