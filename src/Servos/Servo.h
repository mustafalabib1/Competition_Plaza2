#pragma once 
#include "ESP32Servo.h"

/* --- SERVO MOTORS --- */
#define SHOULDER_SERVO      25
#define ELBOW_SERVO         26
#define GRIPPER1_SERVO      32
#define GRIPPER2_SERVO      18
#define WRIST_SERVO         2

Servo Shoulder, Elbow, Gripper1, Gripper2, Wrist;

// servo initial angles
extern int shoulderAngle = 90; // Start at a neutral position
extern int elbowAngle = 90;
extern int gripper1Angle = 90;
extern int gripper2Angle = 90;
extern int wristAngle = 0;

void initServos();