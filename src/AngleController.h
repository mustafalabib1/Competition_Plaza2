#pragma once

#include "motors.h"
#include "RobotState.h"
#include "NonBlockingDelay.h"

// Robot Physical Parameters
#define TRACK_WIDTH 0.199    // in meters
#define WHEEL_DIAMETER 0.065 // in meters
#define MOTOR_RPM 300        // Motor speed in RPM

void rotateDegrees(int degrees);
bool isRotationComplete();
void Right90();
void Left90();
void UTurn();
