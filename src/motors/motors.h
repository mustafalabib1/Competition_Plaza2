#include <Arduino.h>

void MotorsInit();
void setMotor(int pwmPin, int dirPin, int speed) ;
void moveCar(int leftSpeed, int rightSpeed);
void driveMecanum(int x, int y, int rotation);