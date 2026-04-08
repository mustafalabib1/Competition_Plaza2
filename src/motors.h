#ifndef motors_H
#define motors_H

// Front Left Motor
#define FRONT_LEFT_PWM 33
#define FRONT_LEFT_DIR 19

// Front Right Motor
#define FRONT_RIGHT_PWM 4
#define FRONT_RIGHT_DIR 26

// Rear Left Motor (Note: DIR is on Strapping Pin GPIO 5)
#define REAR_LEFT_PWM 16
#define REAR_LEFT_DIR 5

// Rear Right Motor (Note: DIR is on Strapping Pin GPIO 15)
#define REAR_RIGHT_PWM 17
#define REAR_RIGHT_DIR 15
#include <Arduino.h>

void MotorsInit();
void setMotor(int pwmPin, int dirPin, int speed);
void moveCar(int leftSpeed, int rightSpeed);
void driveMecanum(int x, int y, int rotation);

#endif