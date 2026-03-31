#pragma once

#include "motors/motors.h"
#include <PID_v2.h>
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "TOF/TOF.h"
#include "AngleController/AngleController.h"

// Variables to store motor speeds
int rightMotorSpeed = 0, leftMotorSpeed = 0;
// Enum for Maze solving state machine
enum MazeState
{
    MAZE_TURNING_LEFT,
    MAZE_TURNING_RIGHT,
    MAZE_MOVING_FORWARD,
    MAZE_UTURNING
};

MazeState currentMazeState = MAZE_MOVING_FORWARD; // Start by moving forward

void solveMaze();
void solveMazeInit();
void stablilizerControl();
void readSensors();
void decide();
void stablilizerControlTest(char command);