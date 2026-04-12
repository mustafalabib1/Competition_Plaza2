#ifndef motors_H
#define motors_H

// Front Left Motor
#define FRONT_LEFT_PWM 19
#define FRONT_LEFT_DIR 33

// Front Right Motor
#define FRONT_RIGHT_PWM 4
#define FRONT_RIGHT_DIR 26

// Rear Left Motor
#define REAR_LEFT_PWM 5
#define REAR_LEFT_DIR 16

// Rear Right Motor
#define REAR_RIGHT_PWM 17
#define REAR_RIGHT_DIR 15

// LEDC PWM Channels
#define CH_FRONT_LEFT 0
#define CH_FRONT_RIGHT 1
#define CH_REAR_LEFT 2
#define CH_REAR_RIGHT 3

#include <Arduino.h>

void MotorsInit();
void setMotor(int pwmChannel, int dirPin, int speed);
void moveCar(int leftSpeed, int rightSpeed);
void driveMecanum(int x, int y, int rotation);

#endif