#include "AngleController.h"

// Non-blocking delays for better performance
NonBlockingDelay rotationDelay;

void rotateDegrees(int degrees)
{
    int direction = (degrees < 0) ? -1 : 1;
    float arcLength = PI * TRACK_WIDTH * (abs(degrees*1.00) / 360.0);
    float wheelCircumference = PI * WHEEL_DIAMETER;
    float revolutions = arcLength / wheelCircumference;
    float timeSeconds = (revolutions * 60.0) / (MOTOR_RPM * robotState.baseSpeed / 255.0);
    int timeMillis = (int)(timeSeconds * 1000 * robotState.rotationCalibration);

    moveCar(direction * robotState.baseSpeed, -direction * robotState.baseSpeed);
    rotationDelay.start(timeMillis);

    // while (!isRotationComplete())
    // {
    //     // Wait for rotation to complete
    // }
}
bool isRotationComplete()
{
  if (rotationDelay.isFinished())
  {
    moveCar(0, 0); // Stop
    return true;
  }
  return false;
}

void Right90()
{
    rotateDegrees(90);
}

void Left90()
{
    rotateDegrees(-90);
}

void UTurn()
{
    rotateDegrees(180);
}