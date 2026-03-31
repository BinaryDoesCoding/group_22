#pragma once

#include "chassis.h"
#include "grid_defs.h"
#include "HC-SR04.h"
#include "filter.h"

class Robot
{
protected:
    enum ROBOT_STATE
    {
        ROBOT_IDLE,
        ROBOT_DRIVING,
        ROBOT_LINING,
        ROBOT_TURNING,
        ROBOT_STANDOFF
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    Pose currPose;
    Pose destPose;

    GridNavigationState currGridState;
    GridNavigationState destGridState;

    Chassis chassis;

    static const uint8_t ECHO = 11;
    static const uint8_t TRIG = 4;

    HC_SR04 sr04 = HC_SR04(ECHO, TRIG);
    LowPassFilter lowPass = LowPassFilter(0.1);

    float prevDistance = 999.0f;
    bool drivingForward = true;

    static constexpr float STANDOFF_THRESHOLD_CM = 0.0f;
    static constexpr float FORWARD_SPEED_CM_S = 10.0f;
    static constexpr float REVERSE_SPEED_CM_S = -10.0f;

public:
    Robot(void) {}

    void InitializeRobot(void);
    void InitializeDecoder(void);
    void RangerFinderTest(void);
    void RobotLoop(void);
    void RangefinderISR(void);

protected:
    void HandleKeyCode(int16_t keyCode);

    void EnterIdleState(void);
    void EnterStandoffState(void);

    void HandleDistanceReading(float distance);
    bool CheckApproachEvent(float distance);
    bool CheckDepartureEvent(float distance);
    void HandleApproachEvent(void);
    void HandleDepartureEvent(void);

    void EnterLineFollowingState(float);
    void EnterCenteringState(void);
    void EnterTurningState(int8_t);
    void EnterDrivingState(void);

    void HandleIntersection(void);
    bool CheckTurnComplete(void);
    void HandleTurnComplete(void);

    void SetDestination(int, int, float);
    void HandleDestinationReached(void);

    void HandleObjectInRangeEvent(void);

    void HandlePitchUp(void);
    void HandlePitchFlat(void);
};