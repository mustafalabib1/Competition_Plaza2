#include "Servo.h"

void initServos()
{
    Shoulder.attach(SHOULDER_SERVO);
    Elbow.attach(ELBOW_SERVO);
    Gripper1.attach(GRIPPER1_SERVO);
    Gripper2.attach(GRIPPER2_SERVO);
    Wrist.attach(WRIST_SERVO);

    // Move all servos to their initial angles
    Shoulder.write(shoulderAngle);
    Elbow.write(elbowAngle);
    Gripper1.write(gripper1Angle);
    Gripper2.write(gripper2Angle);
    Wrist.write(wristAngle);
}