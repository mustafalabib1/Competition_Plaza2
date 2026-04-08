#pragma once

extern double FrontDistance;
extern double RightDistance;
extern double LeftDistance;

#include "RobotState.h"

void TofInit();
void PowerOffTofSensors();
float getFrontDistance();
float getRightDistance();
float getLeftDistance();
