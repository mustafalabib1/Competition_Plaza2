#include "AngleController.h"

// Non-blocking delays for better performance
NonBlockingDelay rotationDelay;

Turn rightTurn;
Turn leftTurn;

void leftHandInit()
{
  rightTurn.turnTime = 600;
  rightTurn.rightSpeed = -150;
  rightTurn.leftSpeed = 255;
  rightTurn.forwardTime = 350;

  leftTurn.turnTime = 800;
  leftTurn.rightSpeed = -50;
  leftTurn.leftSpeed = 255;
  leftTurn.forwardTime = 250;
}

void rightHandInit()
{
  rightTurn.turnTime = 800;
  rightTurn.rightSpeed = 255;
  rightTurn.leftSpeed = -50;
  rightTurn.forwardTime = 250;

  leftTurn.turnTime = 600;
  leftTurn.rightSpeed = 255;
  leftTurn.leftSpeed = -150;
  leftTurn.forwardTime = 350;
}

void rotateDegrees(int degrees)
{
  int direction = (degrees < 0) ? -1 : 1;
  float arcLength = PI * TRACK_WIDTH * (abs(degrees * 1.00) / 360.0);
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
  unsigned long pretime = millis();

  while (millis() - pretime <= 600)
  {
    moveCar(255, -150);
  }
  pretime = millis();
  while (millis() - pretime <= 350)
  {
    moveCar(255, 255);
  }
  moveCar(0, 0);
}

// rotateDegrees(90);
void Left90()
{
  unsigned long pretime = millis();
  while (millis() - pretime <= 800)
  {
    moveCar(-50, 255);
  }
  pretime = millis();
  while (millis() - pretime <= 250)
  {
    moveCar(255, 255);
  }
  moveCar(0, 0);

  // rotateDegrees(-90);
}

void UTurn()
{
  rotateDegrees(180);
}