#pragma once 

#include "motors/motors.h"
#include "RobotState.h"

void executeCommand(char command);
// RC Command Definitions
#define FORWARD 'F'           // Move forward
#define BACKWARD 'B'          // Move backward
#define LEFT 'R'              // Turn right
#define RIGHT 'L'             // Turn left
#define FORWARD_LEFT 'G'      // Move forward left
#define FORWARD_RIGHT 'H'     // Move forward right
#define BACKWARD_LEFT 'I'     // Move backward left
#define BACKWARD_RIGHT 'J'    // Move backward right
#define STOP 'S'              // Stop all motors
#define HONK 'Y'              // Honk horn
#define HEADLIGHTS_ON 'X'     // Turn headlights ON
#define HEADLIGHTS_OFF 'x'    // Turn headlights OFF