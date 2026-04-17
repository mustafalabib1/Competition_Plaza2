#ifndef MazaSolve_H
#define MazaSolve_H

#include "motors.h"
#include <PID_v2.h>
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "TOF.h"
#include "AngleController.h"

// PID variables for Stabilizer Control
extern double pid_error, pid_setpoint, pid_output;
// Distances
extern double FrontDistance;
extern double RightDistance;
extern double LeftDistance ;
// Variables to store motor speeds
extern int rightMotorSpeed, leftMotorSpeed;
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

#endif