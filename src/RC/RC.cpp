#include "RC.h"

void executeCommand(char command)
{
    switch (command)
    {
    case 'F': // Move forward
        driveMecanum(0, 255, 0);
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
        driveMecanum(0, 0, 0);
        break;
        
    case 'Y': // Honk horn
    case 'X': // Turn headlight ON
    case 'x': // Turn headlight OFF

    default:
        // Invalid command, stop the robot
        driveMecanum(0, 0, 0);
        break;
    }
}