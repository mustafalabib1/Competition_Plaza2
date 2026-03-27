#pragma once 

#include "motors/motors.h"
#include <PID_v2.h>
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "TOF/TOF.h"

void solveMaze();
void solveMazeInit();
void stablilizerControl();
void readSensors();
void decide();