#pragma once

#include <Arduino.h>
struct StTurn
{
    int turnTime;
    int rightSpeed;
    int leftSpeed;
    int forwardTime;
};

extern StTurn rightTurn;
extern StTurn leftTurn;