#include "robot.h"
#include "IRdecoder.h"
#include "TeleplotUtils.h"

extern IRDecoder decoder;
extern Robot robot;

void hcISR(void)
{
    robot.RangefinderISR();
}

void Robot::RangefinderISR(void)
{
    sr04.ISR_echo();
}

void Robot::InitializeRobot(void)
{
    InitializeDecoder();

    /**
     * Initialize the chassis, which includes motors, timer loops, etc.
     */
    chassis.InititalizeChassis();

    pinMode(13, OUTPUT);

    /**
     * Initialize ultrasonic sensor
     */
    sr04.init(hcISR);
}

/**
 * The main loop for your robot. Process both synchronous events (motor control),
 * and asynchronous events (IR presses, distance readings, etc.).
 */
void Robot::RobotLoop(void)
{
    /**
     * Handle any IR remote keypresses.
     */
    int16_t keyCode = decoder.getKeyCode();
    if (keyCode != -1) HandleKeyCode(keyCode);

    /**
     * SpinOnce() performs some low-level chassis computations. It returns true
     * when the motor loop timer is executed, which allows you to synchronize tasks
     * with the motor updates.
     */
    if (chassis.SpinOnce())
    {
        /**
         * Put any motor-synchronous actions here.
         */
        if (robotState == ROBOT_LINING)
        {
            /**
             * TODO: Line control in Lab 3
             */
        }

        /**
         * TODO: For a later lab on odometry, update pose and check for events.
         */
    }

    /**
     * In Lab 2, set up messages and events for distance.
     */
    float distance = 0.0f;
    if (sr04.getDistance(distance))
    {
        HandleDistanceReading(distance);
    }

    /**
     * TODO: In Lab 3, check the line sensor for an intersection
     */

    /**
     * TODO: In Lab 4, we'll set up the IMU and related functionality
     */
}

void Robot::EnterIdleState(void)
{
    chassis.FullStop();
    robotState = ROBOT_IDLE;
}

void Robot::EnterStandoffState(void)
{
    robotState = ROBOT_STANDOFF;
    prevDistance = 999.0f;
    drivingForward = true;

    chassis.SetTwist(FORWARD_SPEED_CM_S, 0.0f);
    digitalWrite(13, LOW);
}

void Robot::HandleDistanceReading(float distance)
{
#ifdef __DIST_DEBUG__
    Serial.print("raw distance: ");
    Serial.println(distance);
#endif

    float filteredDistance = lowPass.CalcFiltered(distance);

#ifdef __DIST_DEBUG__
    Serial.print("filtered distance: ");
    Serial.println(filteredDistance);
#endif

    TeleplotPrint("raw", distance);
    TeleplotPrint("filtered", filteredDistance);

    if (CheckApproachEvent(filteredDistance))
    {
        HandleApproachEvent();
    }
    else if (CheckDepartureEvent(filteredDistance))
    {
        HandleDepartureEvent();
    }

    prevDistance = filteredDistance;
}

bool Robot::CheckApproachEvent(float distance)
{
    return (prevDistance > STANDOFF_THRESHOLD_CM) &&
           (distance <= STANDOFF_THRESHOLD_CM);
}

bool Robot::CheckDepartureEvent(float distance)
{
    return (prevDistance <= STANDOFF_THRESHOLD_CM) &&
           (distance > STANDOFF_THRESHOLD_CM);
}

void Robot::HandleApproachEvent(void)
{
    if (robotState != ROBOT_STANDOFF) return;
    if (!drivingForward) return;

    chassis.SetTwist(REVERSE_SPEED_CM_S, 0.0f);
    drivingForward = false;

    digitalWrite(13, HIGH);
}

void Robot::HandleDepartureEvent(void)
{
    if (robotState != ROBOT_STANDOFF) return;
    if (drivingForward) return;

    chassis.SetTwist(FORWARD_SPEED_CM_S, 0.0f);
    drivingForward = true;

    digitalWrite(13, LOW);
}

/**
 * Enter line following.
 */
void Robot::EnterLineFollowingState(float lineSpeed)
{
    /**
     * TODO: In Lab 3, set baseSpeed and state
     */
}

void Robot::HandleIntersection(void)
{
    // Useful for debugging:
    Serial.println("X");

    /**
     * TODO: In Lab 3, build in navigation logic.
     */
}

void Robot::HandleObjectInRangeEvent(void)
{
    /**
     * TODO: In Lab 2, handle event
     */
}

void Robot::RangerFinderTest(void)
{
    float distance = 0.0f;
    if (sr04.getDistance(distance))
    {
        Serial.print("Distance: ");
        Serial.println(distance);
    }
}