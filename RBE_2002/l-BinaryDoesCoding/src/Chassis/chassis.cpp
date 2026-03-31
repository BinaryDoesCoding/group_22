#include "chassis.h"
#include <Arduino.h>
#include "Romi32U4MotorTemplate.h"

void Chassis::SetMotorKp(float kp) {leftMotor.Kp = kp; rightMotor.Kp = kp;}
void Chassis::SetMotorKi(float ki) {leftMotor.Ki = ki; rightMotor.Ki = ki; leftMotor.sumError = 0; rightMotor.sumError = 0;}
void Chassis::SetMotorKd(float kd) {leftMotor.Kd = kd; rightMotor.Kd = kd;}

/**
 * Because it's declared static, we initialize Chassis::loopFlag here.
 */
volatile uint8_t Chassis::loopFlag = 0;

/**
 * Initialize the timer loop and set up the motor timers and encoders
 */
void Chassis::InititalizeChassis(void)
{
    InitializeMotorControlTimer();
    InitializeMotors();

    pinMode(A1, INPUT);     // for reading battery; defaults to input, but good practice
}

/**
 * For motor control. Every time through the loop (20ms), we'll take a "snapshot" of wheel speeds,
 * which will have units of "encoder ticks / 20ms" and then raise a flag. SpinOnce() checks to see
 * if the flag is raised, and if it is, it processes the motor control from the speeds found here.
 */
void Chassis::Timer4OverflowISRHandler(void)
{
    loopFlag++;

    leftMotor.speed = leftMotor.CalcEncoderDelta();
    rightMotor.speed = rightMotor.CalcEncoderDelta();
}

/**
 * ISR for timing. On Timer4 overflow, we take a 'snapshot' of the encoder counts
 * and raise a flag to let the program it is time to execute the PID calculations.
 */
ISR(TIMER4_OVF_vect)
{
   Chassis::Timer4OverflowISRHandler();
}

/**
 * Sets up a hardware timer on Timer4 to manage motor control on a precise schedule.
 *
 * We set the timer to set an interrupt flag on overflow, which is handled
 * by ISR(TIMER4_OVF_vect).
 */
void Chassis::InitializeMotorControlTimer(void)
{
    Serial.println("InitTimer");
    // Disable interrupts while we mess with the Timer4 registers
    cli();

    // Set up Timer4
    TCCR4A = 0x00; // Disable output to pins
    TCCR4B = 0x0A; // Sets the prescaler -- see pp. 167-8 in datasheet
    TCCR4C = 0x00; // Disables output to pins (but see below for buzzer)
    TCCR4D = 0x00; // Normal mode: count up and roll-over

    /**
     * Calculate TOP based on prescaler and loop duration. Note that loop is in integer ms --
     * there may be some rounding. Multiples of 4 ms will be exact.
     */
    uint16_t top = ((CONTROL_LOOP_PERIOD_MS * 16000ul) >> 9) - 1; // divides by 512

    /**
     * Here we do a little trick to allow full 10-bit register access.
     * We have 2 _bits_ in TC4H that we can use to add capacity to TOP.
     *
     * Note that the maximum period is limited by TOP = 0x3FF. If you want
     * a longer period, you'll need to adjust the pre-scaler.
     *
     * There is no minimum period, but precision breaks down with low values,
     * unless you adjust the pre-scaler, but the encoder resolution is limited,
     * so you only want to go so fast.
     */
    uint8_t highbits = top / 256;
    uint8_t lowbits = top - highbits;
    TC4H = highbits;
    OCR4C = lowbits;

    // Enable overflow interrupt
    TIMSK4 = 0x04;

    /**
     * Uncommenting the following lines will pipe the timer signal to pin 6,
     * which controls the buzzer. The pin will toggle at the loop rate, which
     * allows you to check that the timing is correct. It will also make a lot
     * of noise, so do so sparingly.
     */
    // TCCR4C = 0x04;
    // pinMode(6, OUTPUT);

    // Re-enable interrupts
    sei();

    Serial.println("/InitTimer");
}

/**
 * The main Chassis loop.
 */
bool Chassis::SpinOnce(void)
{
    bool retVal = false;

    if(loopFlag)
    {
        if(loopFlag > 1) Serial.println("Missed an update in Robot::RobotLoop()!");

#ifdef __LOOP_DEBUG__
        Serial.print(millis());
        Serial.print('\n');
#endif

        // motor updates
        UpdateMotors();

        loopFlag = 0;
        retVal = true;
    }

    if(batteryTimer.CheckExpired(true)) // passing true automatically restarts the timer
    {
        HandleBatteryTimer();
    }

    return retVal;
}

/**
 * Some motor methods.
 */
void Chassis::InitializeMotors(void)
{
    Romi32U4MotorBase::InitializePWMTimerAndInterrupts();

    leftMotor.InitializeMotor();
    rightMotor.InitializeMotor();
}

/**
 * SetMotorEfforts is used to set efforts directly and will switch the motors out of speed control mode.
 * Therefore, it should not be used for PI control. ControlMotorSpeed() is used for that.
 *
 * It is called when you call Chassis::FullStop(), so that the integral term doesn't keep moving the Romi
 */
void Chassis::SetMotorEfforts(int16_t left, int16_t right)
{
    leftMotor.SetMotorEffortDirect(left);
    rightMotor.SetMotorEffortDirect(right);
}

/**
 * Calls the PID controller for each motor.
 */
void Chassis::UpdateMotors(void)
{
    leftMotor.ControlMotorSpeed();
    rightMotor.ControlMotorSpeed();
}

/**
 * SetWheelSpeeds converts the linear wheel speeds (axles relative to ground) to motor speeds.
 * Input is in cm/sec; convert to ticks/control interval
 */
void Chassis::SetWheelSpeeds(float leftSpeedCMperSec, float rightSpeedCMperSec)
{
    /**
     * TODO: Add code to convert cm/sec -> encoder ticks/control interval.
     */

    float leftTicksPerLoop  = (leftSpeedCMperSec / DISTANCE_PER_ENCODER_TICK) * (CONTROL_LOOP_PERIOD_MS / 1000.0f);
    float rightTicksPerLoop = (rightSpeedCMperSec / DISTANCE_PER_ENCODER_TICK) * (CONTROL_LOOP_PERIOD_MS / 1000.0f);

    leftMotor.SetTargetSpeed(leftTicksPerLoop);
    rightMotor.SetTargetSpeed(rightTicksPerLoop);
}

/**
 * SetTwist takes a motion (in cm/sec and rad/sec) and converts it to wheel speeds.
 */
void Chassis::SetTwist(float fwd_speed, float ang_speed)
{
    /**
     * Convert speeds to individual wheel speeds in cm/sec
     *
     * TODO: Calculate the left and right wheel speeds.
     *
     * Differential drive:
     *   v_l = u - omega * R
     *   v_r = u + omega * R
     */
    float leftSpeed  = fwd_speed - ang_speed * ROBOT_RADIUS;
    float rightSpeed = fwd_speed + ang_speed * ROBOT_RADIUS;

    SetWheelSpeeds(leftSpeed, rightSpeed);
}

/**
 * SetTwist takes a Twist and calls the forward and angular speed version above.
 */
void Chassis::SetTwist(const Twist& twist)
{
    SetTwist(twist.vel.x, twist.omega.z);
}

Twist Chassis::CalcMotionInLocalFrame(void)
{
    Twist localVelocity;

    /**
     * TODO: Calculate velocities from wheel motion.
     *
     * Note that you can save some floating point maths by converting to a delta,
     * instead of a speed. In that case, you should change the return type
     * to a Pose.
     */
    const float dt = CONTROL_LOOP_PERIOD_MS / 1000.0f;

    const float leftCMPerTick  = 1.0f / LEFT_TICKS_PER_CM;
    const float rightCMPerTick = 1.0f / RIGHT_TICKS_PER_CM;

    // leftMotor.speed and rightMotor.speed are encoder ticks / control interval
    float leftSpeedCMperSec  = (leftMotor.speed * leftCMPerTick) / dt;
    float rightSpeedCMperSec = (rightMotor.speed * rightCMPerTick) / dt;

    // Robot forward and angular velocity
    float u = 0.5f * (leftSpeedCMperSec + rightSpeedCMperSec);
    float omega = (rightSpeedCMperSec - leftSpeedCMperSec) / (2.0f * ROBOT_RADIUS);

    localVelocity.vel.x = u;
    localVelocity.vel.y = 0.0f;
    localVelocity.vel.z = 0.0f;

    localVelocity.omega.x = 0.0f;
    localVelocity.omega.y = 0.0f;
    localVelocity.omega.z = omega;

#ifdef __NAV_DEBUG__
    TeleplotPrint("u", localVelocity.vel.x);
    TeleplotPrint("omega", localVelocity.omega.z);
#endif

    return localVelocity;
}

/**
 * TODO: Implement the battery monitor behaviour described in class.
 */
void Chassis::HandleBatteryTimer(void)
{
    uint16_t adcReading = analogRead(A1);

    float pinVoltage = (adcReading * 5.0f) / 1023.0f;
    float batteryVoltage = pinVoltage * 3.0f;   // A1 reads VIN/3

    const float BATTERY_OFF_V   = 1.0f;
    const float BATTERY_LOW_V   = 7.5f;   // warning threshold
    const float BATTERY_EMPTY_V = 6.0f;   // 0%
    const float BATTERY_FULL_V  = 8.4f;   // 100%, good for rechargeables

    float batteryPercent = ((batteryVoltage - BATTERY_EMPTY_V) / (BATTERY_FULL_V - BATTERY_EMPTY_V)) * 100.0f;
    if (batteryPercent < 0.0f) batteryPercent = 0.0f;
    if (batteryPercent > 100.0f) batteryPercent = 100.0f;

    if ((batteryVoltage > 1.0f) && (batteryVoltage < 7.5f))
    {
        batteryState = BATTERY_LOW;

        // COM4A1..COM4A0 = 0b01
        // Toggle OC4A (pin 13) on compare match
        TCCR4A &= ~((1 << COM4A1) | (1 << COM4A0));
        TCCR4A |= (1 << COM4A0);
    }
    else
    {
        batteryState = BATTERY_OK;

        // COM4A1..COM4A0 = 0b00
        // Stop flashing
        TCCR4A &= ~((1 << COM4A1) | (1 << COM4A0));
    }

    Serial.print(F("Battery: "));
    Serial.print(batteryVoltage, 2);
    Serial.print(F(" V ("));
    Serial.print(batteryPercent, 0);
    Serial.println(F("%)"));

    if (batteryState == BATTERY_LOW)
    {
        Serial.println(F("BATTERY LOW"));
    }
}