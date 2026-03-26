#include "PS4Controller.h"
#include "motors/motors.h"
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "AngleController/AngleController.h"
#include "RC/RC.h"
#include "TOF/TOF.h"
#include "Servos/Servo.h"

// --- Constants for Control ---
// Driving
const float JOYSTICK_DEADZONE = 0.1f; // 10% deadzone for joystick
const float ROTATION_DEADZONE = 0.4f; // 40% deadzone for rotation
const int MAX_MOTOR_SPEED = 255;

// Robot Arm
const int ARM_ANGLE_STEP = 10;
const int ARM_MIN_ANGLE = 0;
const int ARM_MAX_ANGLE = 180;

// Gripper 1 (Standard Servo)
const int GRIPPER_OPEN_ANGLE = 90;
const int GRIPPER_CLOSED_ANGLE = 0;

// Gripper 2 (Continuous Rotation Servo)
const int GRIPPER_CONTINUOUS_FORWARD = 135; // Spin forward
const int GRIPPER_CONTINUOUS_BACKWARD = 45; // Spin backward
const int GRIPPER_CONTINUOUS_STOP = 90;     // Stop spinning

// --- Global Variables ---
bool isAutonomousMode = false; // Start in teleop mode

// --- Function Prototypes ---
void handleModeSwitching();
void handleTeleopDrive();
void handleRobotArmControl();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  initializeRobotState();
  MotorsInit();
  initServos();
  TofInit();

  PS4.begin("5c:96:56:af:ad:a0"); // Replace with your ESP32's MAC address
  Serial.println("Waiting for PS4 controller to connect...");
}

void loop()
{
  if (PS4.isConnected())
  {
    handleModeSwitching();

    if (isAutonomousMode)
    {
      // Autonomous mode logic here (not implemented in this example)
    }
    else
    {
      // --- Teleop Control Logic ---
      handleTeleopDrive();
      handleRobotArmControl();
    }
  }
}

void handleModeSwitching()
{
  // Use a static variable to track the previous state of the PS button
  // to detect only the button press event, not holding it down.
  static bool lastPsButtonState = false;
  bool currentPsButtonState = PS4.PSButton();

  if (currentPsButtonState && !lastPsButtonState)
  {
    isAutonomousMode = !isAutonomousMode; // Toggle the mode
    if (isAutonomousMode)
    {
      Serial.println("Switched to Autonomous Mode");
      // Stop motors when switching modes to prevent unexpected movement
      moveCar(0, 0);
    }
    else
    {
      Serial.println("Switched to Teleop Mode");
    }
  }
  lastPsButtonState = currentPsButtonState;
}

void handleTeleopDrive()
{
  // Read and normalize joystick values
  float y = constrain(-PS4.LStickY() / 127.0f, -1.0f, 1.0f);
  float x = constrain(PS4.LStickX() / 127.0f, -1.0f, 1.0f);
  float rotation = constrain(PS4.RStickX() / 127.0f, -1.0f, 1.0f);

  // Apply deadzone to prevent drift from slight joystick movements
  if (fabs(y) < JOYSTICK_DEADZONE)
    y = 0;
  if (fabs(x) < JOYSTICK_DEADZONE)
    x = 0;
  if (fabs(rotation) < ROTATION_DEADZONE)
    rotation = 0;

  // Apply a non-linear response curve (squaring the input) for finer control at low speeds
  y = y * fabs(y);
  x = x * fabs(x);
  rotation = rotation * fabs(rotation);

  // Convert normalized values to motor speeds and drive the robot
  driveMecanum(x * MAX_MOTOR_SPEED, y * MAX_MOTOR_SPEED, rotation * MAX_MOTOR_SPEED);
}
void handleRobotArmControl()
{
  // --- Shoulder Control ---
  if (PS4.Up())
    shoulderAngle += ARM_ANGLE_STEP;
  if (PS4.Down())
    shoulderAngle -= ARM_ANGLE_STEP;

  // --- Elbow Control ---
  if (PS4.Triangle())
    elbowAngle += ARM_ANGLE_STEP;
  if (PS4.Cross())
    elbowAngle -= ARM_ANGLE_STEP;

  // --- Gripper 1 (Standard Servo) ---
  if (PS4.Right())
    gripper1Angle = GRIPPER_OPEN_ANGLE;
  if (PS4.Left())
    gripper1Angle = GRIPPER_CLOSED_ANGLE;

  // --- Wrist Control ---
  if (PS4.R1())
    wristAngle += ARM_ANGLE_STEP;
  if (PS4.L1())
    wristAngle -= ARM_ANGLE_STEP;

  // --- Gripper 2 (Continuous Rotation Servo) ---
  if (PS4.R2())
    gripper2Angle = GRIPPER_CONTINUOUS_FORWARD; // Spin forward
  else if (PS4.L2())
    gripper2Angle = GRIPPER_CONTINUOUS_BACKWARD; // Spin backward
  else
    gripper2Angle = GRIPPER_CONTINUOUS_STOP; // Stop spinning

  // Constrain all angles to prevent servo damage
  shoulderAngle = constrain(shoulderAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
  elbowAngle = constrain(elbowAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
  wristAngle = constrain(wristAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
  // gripper1Angle is set to specific values, but constraining is good practice
  gripper1Angle = constrain(gripper1Angle, GRIPPER_CLOSED_ANGLE, GRIPPER_OPEN_ANGLE);

  // Write angles to servos
  Shoulder.write(shoulderAngle);
  Elbow.write(elbowAngle);
  Gripper1.write(gripper1Angle);
  Gripper2.write(gripper2Angle);
  Wrist.write(wristAngle);
}
