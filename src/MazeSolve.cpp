#include "MazeSolve.h"

// --- Constants for Maze Solving ---
// PID variables for Stabilizer Control
double pid_error, pid_setpoint = 0.0, pid_output;

PID stabilizerPID(&pid_error, &pid_output, &pid_setpoint, robotState.kp, robotState.ki, robotState.kd, DIRECT);

// Variables to store motor speeds
int rightMotorSpeed = 0, leftMotorSpeed = 0;
MazeState currentMazeState = MAZE_MOVING_FORWARD;

void Swap(double &a, double &b);

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
    rightMotorSpeed = robotState.baseSpeed + (int)pid_output;
    leftMotorSpeed = robotState.baseSpeed - (int)pid_output;
    // Constrain motor speeds to valid range
    rightMotorSpeed = constrain(rightMotorSpeed, 0, 250);
    leftMotorSpeed = constrain(leftMotorSpeed, 0, 250);
}

void readSensors()
{
    // Read TOF sensors and update distances
    LeftDistance = getLeftDistance();
    RightDistance = getRightDistance();
    FrontDistance = getFrontDistance();
    if (!robotState.isLeftHandSide)
        Swap(LeftDistance, RightDistance); // If using right-hand side, swap left and right distances for logic consistency
}
void Swap(double &a, double &b)
{
    double temp = a;
    a = b;
    b = temp;
}
void decide()
{
    if (currentMazeState == MAZE_TURNING_RIGHT || currentMazeState == MAZE_TURNING_LEFT || currentMazeState == MAZE_UTURNING)
    {
        // check if rotation is't complete
        if (isRotationComplete())
        {
            return; // Wait until rotation is complete before making new decisions
        }
    }
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
    // Implement maze solving logic here
    switch (currentMazeState)
    {
    case MAZE_TURNING_LEFT:
        Left90();
        break;
    case MAZE_TURNING_RIGHT:
        Right90();
        break;
    case MAZE_MOVING_FORWARD:
        moveCar(rightMotorSpeed, leftMotorSpeed);
        break;
    case MAZE_UTURNING:
        UTurn();
        break;
    default:
        break;
    }
}

void stablilizerControlTest(char command)
{
    switch (command)
    {
    case 'F': // Move forward
        stablilizerControl();
        moveCar(rightMotorSpeed, leftMotorSpeed);
        break;
    case 'B': // Move backward
        driveMecanum(0, -255, 0);
        break;
    case 'R': // Turn right
        driveMecanum(255, 0, 0);
        break;
    case 'L': // Turn left
        driveMecanum(-255, 0, 0);
        break;
    case 'G': // Forward left
        driveMecanum(-255, 255, 0);
        break;
    case 'H': // Forward right
        driveMecanum(255, -255, 0);
        break;
    case 'I': // Backward left
        driveMecanum(-255, -255, 0);
        break;
    case 'J': // Backward right
        driveMecanum(255, 255, 0);
        break;
    case 'S': // Stop all motors
        moveCar(0, 0);
        break;

    case 'Y': // Honk horn
    case 'X': // Turn headlight ON
    case 'x': // Turn headlight OFF

    default:
        // Invalid command, stop the robot
        moveCar(0, 0);
        break;
    }
}