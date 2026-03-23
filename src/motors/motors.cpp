#include "motors.h"

void MotorsInit()
{
  // Set motor control pins as outputs
  pinMode(FRONT_LEFT_PWM, OUTPUT);
  pinMode(FRONT_LEFT_DIR, OUTPUT);

  pinMode(FRONT_RIGHT_PWM, OUTPUT);
  pinMode(FRONT_RIGHT_DIR, OUTPUT);

  pinMode(REAR_LEFT_PWM, OUTPUT);
  pinMode(REAR_LEFT_DIR, OUTPUT);

  pinMode(REAR_RIGHT_PWM, OUTPUT);
  pinMode(REAR_RIGHT_DIR, OUTPUT);

  // Initialize all motors to stopped state
  digitalWrite(FRONT_LEFT_DIR, LOW);
  analogWrite(FRONT_LEFT_PWM, 0);

  digitalWrite(FRONT_RIGHT_DIR, LOW);
  analogWrite(FRONT_RIGHT_PWM, 0);

  digitalWrite(REAR_LEFT_DIR, LOW);
  analogWrite(REAR_LEFT_PWM, 0);

  digitalWrite(REAR_RIGHT_DIR, LOW);
  analogWrite(REAR_RIGHT_PWM, 0);
}

void setMotor(int pwmPin, int dirPin, int speed)
{
  // Constraints speed to the valid PWM range (-255 to 255)
  speed = constrain(speed, -255, 255);
  digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
  analogWrite(pwmPin, speed >= 0 ? 255 - speed : -speed);
}

void moveCar(int leftSpeed, int rightSpeed)
{
  setMotor(FRONT_LEFT_PWM, FRONT_LEFT_DIR, leftSpeed);
  setMotor(REAR_LEFT_PWM, REAR_LEFT_DIR, leftSpeed);
  setMotor(FRONT_RIGHT_PWM, FRONT_RIGHT_DIR, rightSpeed);
  setMotor(REAR_RIGHT_PWM, REAR_RIGHT_DIR, rightSpeed);
}
// Main function to drive the mecanum robot
// Inputs should be between -255 and 255
void driveMecanum(int x, int y, int rotation)
{

  // Calculate the speed for each wheel using mecanum kinematics
  int frontLeftSpeed = y + x + rotation;
  int frontRightSpeed = y - x - rotation;
  int rearLeftSpeed = y - x + rotation;
  int rearRightSpeed = y + x - rotation;

  // Find the maximum value among the calculated speeds
  // This ensures we keep the correct movement proportions if math exceeds 255
  int maxSpeed = max(max(abs(frontLeftSpeed), abs(frontRightSpeed)),
                     max(abs(rearLeftSpeed), abs(rearRightSpeed)));

  // If any speed is above 255, scale all speeds down proportionally
  if (maxSpeed > 255)
  {
    frontLeftSpeed = (frontLeftSpeed * 255) / maxSpeed;
    frontRightSpeed = (frontRightSpeed * 255) / maxSpeed;
    rearLeftSpeed = (rearLeftSpeed * 255) / maxSpeed;
    rearRightSpeed = (rearRightSpeed * 255) / maxSpeed;
  }

  // Send the calculated speeds to the motors using our helper function
  setMotor(FRONT_LEFT_PWM, FRONT_LEFT_DIR, frontLeftSpeed);
  setMotor(FRONT_RIGHT_PWM, FRONT_RIGHT_DIR, frontRightSpeed);
  setMotor(REAR_LEFT_PWM, REAR_LEFT_DIR, rearLeftSpeed);
  setMotor(REAR_RIGHT_PWM, REAR_RIGHT_DIR, rearRightSpeed);
}