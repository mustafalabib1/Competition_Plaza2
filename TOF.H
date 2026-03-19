#ifndef TOF_H
#define TOF_H

#include <Arduino.h>

void TofInit();

float getFrontDistance();
float getRightDistance();
float getLeftDistance();

#endif