// robot.cpp - Main robot class implementation. This is where the bulk of the logic for the robot will go. It will manage the chassis and sensors and implement the state machine for the robot's behavior.
#include "robot.h"
#include "IRdecoder.h"
#include "TeleplotUtils.h"
#include <math.h>

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

void Robot::SetSensorMode(DIST_SENSOR_MODE mode)
{
    sensorMode = mode;
}

void Robot::InitializeRobot(void)
{
    InitializeDecoder();

    chassis.InititalizeChassis();

    pinMode(13, OUTPUT);

    // Ultrasonic setup
    sr04.init(hcISR);

    // Sharp IR setup
    sharpIR.init();

    // Line sensor setup
    lineSensor.Init();
}

void Robot::RobotLoop(void)
{
    int16_t keyCode = decoder.getKeyCode();
    if (keyCode != -1)
    {
        HandleKeyCode(keyCode);
    }

    unsigned long now = millis();

    /**
     * Check for intersections only while line following.
     * Count only the rising edge, and ignore retriggers briefly.
     */
    if (robotState == ROBOT_LINING)
    {
        bool currentIntersectionState = lineSensor.CheckIntersection();

        TeleplotPrint("intersection_state", currentIntersectionState ? 1.0f : 0.0f);
        TeleplotPrint("intersection_count", (float)intersectionCount);

        if ((now >= intersectionIgnoreUntilMs) &&
            currentIntersectionState &&
            !lastIntersectionState)
        {
            HandleIntersection();
            intersectionIgnoreUntilMs = now + INTERSECTION_RETRIGGER_IGNORE_MS;
        }

        lastIntersectionState = currentIntersectionState;
    }
    else
    {
        lastIntersectionState = false;
    }

    if (chassis.SpinOnce())
    {
        if (robotState == ROBOT_LINING)
        {
            int leftRaw = analogRead(LINE_LEFT_PIN);
            int rightRaw = analogRead(LINE_RIGHT_PIN);
            float lineError = lineSensor.CalcError();
            float turnEffort = LINE_FOLLOW_KP * lineError;

            chassis.SetTwist(lastLineFollowSpeed, turnEffort);

            TeleplotPrint("line_left_raw", (float)leftRaw);
            TeleplotPrint("line_right_raw", (float)rightRaw);
            TeleplotPrint("line_error", lineError);
            TeleplotPrint("line_turn_effort", turnEffort);
        }

        // TODO: Odometry / other synchronous tasks later
    }

    bool newReading = false;
    float distanceReading = 0.0f;

    if (sensorMode == SENSOR_SHARP_IR)
    {
        newReading = sharpIR.getDistance(distanceReading);
    }
    else if (sensorMode == SENSOR_ULTRASONIC)
    {
        newReading = sr04.getDistance(distanceReading);
    }

    if (newReading)
    {
        HandleDistanceReading(distanceReading);
    }
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

void Robot::EnterAligningState(void)
{
    robotState = ROBOT_ALIGNING;
    chassis.SetTwist(0.0f, 0.0f);
    digitalWrite(13, LOW);
}

void Robot::HandleDistanceReading(float distance)
{
#ifdef __DIST_DEBUG__
    Serial.print("raw distance: ");
    Serial.println(distance);
#endif

    // Reject impossible values before filtering
    if (distance < 0.0f || distance > 400.0f)
    {
#ifdef __DIST_DEBUG__
        Serial.print("Rejected distance: ");
        Serial.println(distance);
#endif
        return;
    }

    float filteredDistance = lowPass.CalcFiltered(distance);

#ifdef __DIST_DEBUG__
    Serial.print("filtered distance: ");
    Serial.println(filteredDistance);
#endif

    TeleplotPrint("range_raw", distance);
    TeleplotPrint("range_filtered", filteredDistance);

    if (sensorMode == SENSOR_SHARP_IR)
    {
        TeleplotPrint("sensor_mode", 1);
    }
    else
    {
        TeleplotPrint("sensor_mode", 2);
    }

    if (robotState == ROBOT_STANDOFF)
    {
        if (CheckApproachEvent(filteredDistance))
        {
            HandleApproachEvent();
        }
        else if (CheckDepartureEvent(filteredDistance))
        {
            HandleDepartureEvent();
        }
    }

    if (robotState == ROBOT_ALIGNING)
    {
        HandleAlignment(filteredDistance);
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

bool Robot::CheckAlignment(float distance)
{
    return fabs(distance - ALIGN_DISTANCE_CM) <= ALIGN_TOLERANCE_CM;
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

void Robot::HandleAlignment(float distance)
{
    if (robotState != ROBOT_ALIGNING) return;

    float error = distance - ALIGN_DISTANCE_CM;

    const float kp = 3.0f;
    const float minSpeed = 0.5f;

    float speed = kp * error;

    if (fabs(error) <= ALIGN_TOLERANCE_CM)
    {
        chassis.SetTwist(0.0f, 0.0f);
        digitalWrite(13, LOW);
        return;
    }

    if (speed > FORWARD_SPEED_CM_S) speed = FORWARD_SPEED_CM_S;
    if (speed < REVERSE_SPEED_CM_S) speed = REVERSE_SPEED_CM_S;

    if (speed > 0.0f && speed < minSpeed) speed = minSpeed;
    if (speed < 0.0f && speed > -minSpeed) speed = -minSpeed;

    chassis.SetTwist(speed, 0.0f);
    digitalWrite(13, HIGH);
}

void Robot::EnterLineFollowingState(float lineSpeed)
{
    robotState = ROBOT_LINING;
    lastLineFollowSpeed = lineSpeed;
    lastIntersectionState = false;
    intersectionIgnoreUntilMs = 0;
    intersectionCount = 0;
    chassis.SetTwist(lineSpeed, 0.0f);

    Serial.println("Robot -> LINE FOLLOW");
}

void Robot::EnterCenteringState(void)
{
    // TODO
}

void Robot::EnterTurningState(int8_t dir)
{
    (void)dir;
    robotState = ROBOT_TURNING;
}

void Robot::EnterDrivingState(void)
{
    robotState = ROBOT_DRIVING;
}

void Robot::HandleIntersection(void)
{
    intersectionCount++;

    digitalWrite(13, !digitalRead(13));

    Serial.print("Intersection count: ");
    Serial.println(intersectionCount);

    if (intersectionCount >= TARGET_INTERSECTION_COUNT)
    {
        Serial.println("Reached third intersection -> IDLE");
        EnterIdleState();
    }
}

bool Robot::CheckTurnComplete(void)
{
    // TODO
    return false;
}

void Robot::HandleTurnComplete(void)
{
    // TODO
}

void Robot::SetDestination(int x, int y, float theta)
{
    (void)x;
    (void)y;
    (void)theta;
    // TODO
}

void Robot::HandleDestinationReached(void)
{
    EnterIdleState();
}

void Robot::HandleObjectInRangeEvent(void)
{
    // TODO
}

void Robot::HandlePitchUp(void)
{
    // TODO
}

void Robot::HandlePitchFlat(void)
{
    // TODO
}

void Robot::RangerFinderTest(void)
{
    float testDistance = 0.0f;
    bool ok = false;

    if (sensorMode == SENSOR_SHARP_IR)
    {
        ok = sharpIR.getDistance(testDistance);
    }
    else
    {
        ok = sr04.getDistance(testDistance);
    }

    if (ok)
    {
        Serial.print("Distance: ");
        Serial.println(testDistance);
    }
}