#include "motors.h"

// Front Left Motor
#define FRONT_LEFT_PWM 33
#define FRONT_LEFT_DIR 19

// Front Right Motor
#define FRONT_RIGHT_PWM 4
#define FRONT_RIGHT_DIR 23

// Rear Left Motor (Note: DIR is on Strapping Pin GPIO 5)
#define REAR_LEFT_PWM 16
#define REAR_LEFT_DIR 5

// Rear Right Motor (Note: DIR is on Strapping Pin GPIO 15)
#define REAR_RIGHT_PWM 17
#define REAR_RIGHT_DIR 15

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
  digitalWrite(dirPin, speed >= 0);
  analogWrite(pwmPin, abs(speed));
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