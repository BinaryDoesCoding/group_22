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

    // only if we're idle
    if(state == IDLE)
    {
      // check that at least 10 ms has expired since the echo was received to let echoes die out
      if(currTime - lastPingCheck >= 10)
      {
          lastPingCheck = currTime; // update lastPingCheck

          //disable interrupts while we adjust the ISR variables
          cli();
          pulseEnd = pulseStart = 0;

          //set state to sent
          state = PING_SENT;
          sei();

#ifdef __HC_DEBUG__
          Serial.println("Ping");
#endif

          // toggle the trigger pin to send a chirp
          digitalWrite(trigPin, HIGH); //commands a ping; leave high for the duration
          delayMicroseconds(12); //datasheet says hold HIGH for 10us; resolution is 4us
          digitalWrite(trigPin, LOW); //pin must be brought LOW

          pingSent = true;
      }
    }

    // this will force a wait until 10 ms after the first time the device is idle
    else lastPingCheck = currTime; // update lastPingCheck

    return pingSent;
}

bool HC_SR04::getDistance(float& distance)
{
    bool retVal = false;

    if(state == ERROR)
    {
      lastPingCheck = millis();
      state = IDLE;
    }

    uint32_t echoLength = 0;

    if(state & ECHO_RECD)
    {
        cli();
        echoLength = (pulseEnd - pulseStart);
        state = IDLE;
        sei();

        lastPingCheck = millis(); // this will assert a pause after receiving an echo

        /**
         * TODO: Filter out any pings that are longer than the maximum range
         */

        /**
         * TODO: Add conversion factor! Don't forget to update the retVal!
         */

        distance = (echoLength - 1251.33) / 36.85; // convert from us to cm!
        retVal = true;
    }
    
    /**
     * By default, we auto-ping. Change this if you don't want to.
     */
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