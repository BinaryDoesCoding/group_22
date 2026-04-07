// robot.h - Main robot class. This is where the bulk of the logic for the robot will go. It will manage the chassis and sensors and implement the state machine for the robot's behavior.
#pragma once

#include "chassis.h"
#include "grid_defs.h"
#include "HC-SR04.h"
#include "filter.h"
#include "Sharp-IR.h"
#include "line_sensor.h"

class Robot
{
protected:
    enum ROBOT_STATE
    {
        ROBOT_IDLE,
        ROBOT_DRIVING,
        ROBOT_LINING,
        ROBOT_TURNING,
        ROBOT_STANDOFF,
        ROBOT_ALIGNING
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    enum DIST_SENSOR_MODE
    {
        SENSOR_ULTRASONIC,
        SENSOR_SHARP_IR
    };
    DIST_SENSOR_MODE sensorMode = SENSOR_SHARP_IR;

    Pose currPose;
    Pose destPose;

    GridNavigationState currGridState;
    GridNavigationState destGridState;

    Chassis chassis;

    static const uint8_t ECHO = 11;
    static const uint8_t TRIG = 4;
    static const uint8_t SHARP_IR_PIN = A0;

    /**
     * QTR wiring:
     * sensor board pin 5 -> Romi A6
     * sensor board pin 3 -> Romi A2
     *
     * This ordering assumes:
     * A6 = left sensor
     * A2 = right sensor
     *
     * If line following turns the wrong way, swap these or flip CalcError().
     */
    static const uint8_t LINE_LEFT_PIN = A6;
    static const uint8_t LINE_RIGHT_PIN = A2;

    /**
     * Thresholds tuned from your current plots.
     */
    static constexpr unsigned int LINE_LIGHT_THRESHOLD = 250;
    static constexpr unsigned int LINE_DARK_THRESHOLD = 650;

    /**
     * Starter line-following parameters.
     * Tune these on the real robot.
     */
    static constexpr float LINE_FORWARD_SPEED_CM_S = 40.0f;
    static constexpr float LINE_FOLLOW_KP = 0.02f;

    /**
     * Ignore window after detecting an intersection so one crossing
     * does not get counted multiple times.
     */
    static constexpr unsigned long INTERSECTION_RETRIGGER_IGNORE_MS = 700;
    static constexpr uint8_t TARGET_INTERSECTION_COUNT = 3;

    HC_SR04 sr04 = HC_SR04(ECHO, TRIG);
    SharpIR sharpIR;
    PololuLineSensor lineSensor = PololuLineSensor(
        LINE_LEFT_PIN,
        LINE_RIGHT_PIN,
        LINE_LIGHT_THRESHOLD,
        LINE_DARK_THRESHOLD
    );
    LowPassFilter lowPass = LowPassFilter(0.1f);

    float prevDistance = 999.0f;
    bool drivingForward = true;

    static constexpr float STANDOFF_THRESHOLD_CM = 30.0f;
    static constexpr float FORWARD_SPEED_CM_S = 30.0f;
    static constexpr float REVERSE_SPEED_CM_S = -30.0f;
    static constexpr float ALIGN_DISTANCE_CM = 15.0f;
    static constexpr float ALIGN_TOLERANCE_CM = 0.75f;

    /**
     * Intersection counting state.
     */
    bool lastIntersectionState = false;
    unsigned long intersectionIgnoreUntilMs = 0;
    uint8_t intersectionCount = 0;
    float lastLineFollowSpeed = LINE_FORWARD_SPEED_CM_S;

public:
    Robot(void) : sharpIR(SHARP_IR_PIN) {}

    void InitializeRobot(void);
    void InitializeDecoder(void);
    void RangerFinderTest(void);
    void RobotLoop(void);
    void RangefinderISR(void);
    void SetSensorMode(DIST_SENSOR_MODE mode);

protected:
    void HandleKeyCode(int16_t keyCode);

    void EnterIdleState(void);
    void EnterStandoffState(void);
    void EnterAligningState(void);

    void HandleDistanceReading(float distance);
    bool CheckApproachEvent(float distance);
    bool CheckDepartureEvent(float distance);
    bool CheckAlignment(float distance);
    void HandleApproachEvent(void);
    void HandleDepartureEvent(void);
    void HandleAlignment(float distance);

    void EnterLineFollowingState(float lineSpeed);
    void EnterCenteringState(void);
    void EnterTurningState(int8_t dir);
    void EnterDrivingState(void);

    void HandleIntersection(void);

    bool CheckTurnComplete(void);
    void HandleTurnComplete(void);

    void SetDestination(int x, int y, float theta);
    void HandleDestinationReached(void);

    void HandleObjectInRangeEvent(void);

    void HandlePitchUp(void);
    void HandlePitchFlat(void);
};