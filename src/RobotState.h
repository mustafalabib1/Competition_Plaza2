#pragma once

#include <Arduino.h>

// Robot State Structure
struct RobotState
{
  // Motor parameters
  int baseSpeed = 200;
  int rightRatio = 100;
  int leftRatio = 100;

  // Calibration parameters
  float leftCalibrationBase = 0.0;
  float rightCalibrationBase = 0.0;
  float frontCalibrationBase = 0.0;
  float leftCalibrationFactor = 1.0;
  float rightCalibrationFactor = 1.0;
  float frontCalibrationFactor = 1.0;
  float rotationCalibration = 1.0;

  int uturnAngle = 180;
  // PID parameters
  float kp = 1.0;
  float ki = 0.0;
  float kd = 0.0;

  // Maze solving state
  bool isLeftHandSide = true; // Default to left-hand side

  float frontThreshold = 20.0; // Distance threshold for obstacle detection in cm
  float leftThreshold = 20.0;
  float rightThreshold = 20.0;


  // EEPROM address tracking
  unsigned int eepromAddress = 0;
};

// Global robot state instance
extern RobotState robotState;

// State management functions
void initializeRobotState();
void saveRobotStateToEEPROM();
void loadRobotStateFromEEPROM();
