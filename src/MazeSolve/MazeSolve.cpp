#include "MazeSolve.h"

// --- Constants for Maze Solving ---
// PID variables for Stabilizer Control
double pid_error, pid_setpoint = 0.0, pid_output;
double LeftDistance, RightDistance, FrontDistance;

PID stabilizerPID(&pid_error, &pid_output, &pid_setpoint, robotState.kp, robotState.ki, robotState.kd, DIRECT);

// Enum for Maze solving state machine
enum MazeState
{
    MAZE_TURNING_LEFT,
    MAZE_TURNING_RIGHT,
    MAZE_MOVING_FORWARD,
    MAZE_UTURNING,
    MAZE_CRASHED
};

MazeState currentMazeState = MAZE_MOVING_FORWARD; // Start by moving forward

void solveMazeInit()
{
    // Initialize PID controller for stabilizer control
    stabilizerPID.SetMode(AUTOMATIC);
    stabilizerPID.SetOutputLimits(-255, 255); // Assuming motor speed range
}
void stablilizerControl()
{
    // Update PID error and compute output
    pid_error = LeftDistance - RightDistance; // Need to maintain equal distance from left and right walls
    stabilizerPID.Compute();

    // Use pid_output to adjust motor speeds for stabilization
    moveCar(robotState.baseSpeed + pid_output, robotState.baseSpeed - pid_output);
}

void readSensors()
{
    // Read TOF sensors and update distances
    LeftDistance = getLeftDistance();
    RightDistance = getRightDistance();
    FrontDistance = getFrontDistance();
    if(!robotState.isLeftHandSide)
    swap(LeftDistance, RightDistance); // If using right-hand side, swap left and right distances for logic consistency
}
void swap(double &a, double &b)
{
    double temp = a;
    a = b;
    b = temp;
}
void decide()
{
    // Implement maze solving logic here
    if (LeftDistance > robotState.frontThreshold)
    {
        currentMazeState = MAZE_TURNING_LEFT;
    }
    else if (FrontDistance > robotState.frontThreshold)
    {
        currentMazeState = MAZE_MOVING_FORWARD;
    }
    else if (RightDistance > robotState.frontThreshold)
    {
        currentMazeState = MAZE_TURNING_RIGHT;
    }
    else
    {
        currentMazeState = MAZE_UTURNING;
    }
}
void solveMaze()
{
    readSensors();
    stablilizerControl();
    decide();
}