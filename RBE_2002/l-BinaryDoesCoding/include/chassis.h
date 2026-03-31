#pragma once

#include <Arduino.h>
#include <event_timer.h>

#include "chassis_params.h"
#include "nav_defs.h"

class Chassis
{
protected:
    /**
     * You can change the control loop period, but you should use multiples of 4 ms to 
     * avoid rounding errors.
     */
    const uint16_t CONTROL_LOOP_PERIOD_MS = 20;

    /**
     * loopFlag is used to tell the program when to update. It is set when Timer4
     * overflows (see InitializeMotorControlTimer). Some of the calculations are too
     * lengthy for an ISR, so we set a flag and use that to key the calculations.
     * 
     * Note that we use a counter so we can see if we've missed a loop. If loopFlag is
     * more than 1, then we missed a cycle.
     * 
     * Static to make it easier to call from an ISR.
     */
    static volatile uint8_t loopFlag;
    
    /**
     * For monitoring the battery. When the battery is low, pin 13 will flash rapidly.
     * It's a little hard to see when it flashes so quickly, but it does kind of draw
     * your attention.
     * 
     * Note that we also identify an "off" state so that it doesn't flash when the
     * power switch is turned off.
     * 
     * We use a software timer to check the battery (once per second). We hardly need to
     * check it any faster than that.
     */
    enum BATTERY_STATE {BATTERY_OFF, BATTERY_LOW, BATTERY_OK};
    BATTERY_STATE batteryState = BATTERY_OK; // Start in OK mode -- I'm sure those batteries are charged

    EventTimer batteryTimer; 
    const uint16_t BATTERY_INTERVAL = 1000;

public:
    Chassis(void) { batteryTimer.Start(1000); }

    void InititalizeChassis(void);

    // For setting motor control gains
    void SetMotorKp(float kp);
    void SetMotorKi(float ki);
    void SetMotorKd(float kd);

    /* Where the bulk of the work for the motors gets done. */
    bool SpinOnce(void);

    /* Needed for managing motors. */
    static void Timer4OverflowISRHandler(void);

    /* Converts u,omega to wheel speeds */
    void SetTwist(float fwdSpeedCMsec, float angVel);
    void SetTwist(const Twist&);

    /**
     * A utility function for converting robot speed to wheel speed. Left 
     * public so that you can test more easily. SetTwist is more convenient.
     */
    void SetWheelSpeeds(float, float);

    /**
     * FullStop() sets motor _efforts_, which will disable the PID motor control.
     * Good for when you want to stop and you don't want the integral term to
     * make it creep forward.
     */
    void FullStop(void) { SetMotorEfforts(0, 0); }

    /**
     * Calculates the motion of the Romi as a Twist
     */
    Twist CalcMotionInLocalFrame(void);

protected:
    /**
     * Initialization and Setup routines
     */
    void InitializeMotorControlTimer(void);
    void InitializeMotors(void);
    
    /**
     * Motor control routines
     */
    void UpdateMotors(void);
    void SetMotorEfforts(int16_t, int16_t); // Do not call this directly!

    /**
     * Battery monitor
     */
    void HandleBatteryTimer(void);
};