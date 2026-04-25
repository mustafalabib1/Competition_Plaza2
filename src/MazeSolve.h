#pragma once

#include "motors.h"
#include <PID_v2.h>
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "TOF.h"
#include "AngleController.h"

// Variables to store motor speeds
extern int rightMotorSpeed, leftMotorSpeed;
extern double pid_error;
// Enum for Maze solving state machine
enum MazeState
{
    MAZE_TURNING_LEFT,
    MAZE_TURNING_RIGHT,
    MAZE_MOVING_FORWARD,
    MAZE_UTURNING
};

extern MazeState currentMazeState;

void solveMaze();
void solveMazeInit();
void stablilizerControl();
void readSensors();
void decide();
void stablilizerControlTest(char command);