#include "RobotState.h"
#include <EEPROM.h>

// Global robot state instance
RobotState robotState;

void initializeRobotState()
{
  // Initialize with default values
  robotState.baseSpeed = 200;
  robotState.rightRatio = 100;
  robotState.leftRatio = 100;
  robotState.rotationCalibration = 1.0;
  robotState.kp = 1.0;
  robotState.ki = 0.0;
  robotState.kd = 0.0;
  robotState.uturnAngle = 180;
  robotState.isLeftHandSide = true;
  robotState.frontThreshold = 20.0;
  robotState.leftThreshold = 20.0;
  robotState.rightThreshold = 20.0;

  // Initialize EEPROM
  EEPROM.begin(512);
}

void saveRobotStateToEEPROM()
{
  robotState.eepromAddress = 0;

  EEPROM.put(robotState.eepromAddress, robotState.baseSpeed);
  robotState.eepromAddress += sizeof(robotState.baseSpeed);

  EEPROM.put(robotState.eepromAddress, robotState.rightRatio);
  robotState.eepromAddress += sizeof(robotState.rightRatio);

  EEPROM.put(robotState.eepromAddress, robotState.leftRatio);
  robotState.eepromAddress += sizeof(robotState.leftRatio);

  EEPROM.put(robotState.eepromAddress, robotState.rotationCalibration);
  robotState.eepromAddress += sizeof(robotState.rotationCalibration);

  EEPROM.put(robotState.eepromAddress, robotState.kp);
  robotState.eepromAddress += sizeof(robotState.kp);

  EEPROM.put(robotState.eepromAddress, robotState.kd);
  robotState.eepromAddress += sizeof(robotState.kd);

  EEPROM.put(robotState.eepromAddress, robotState.ki);
  robotState.eepromAddress += sizeof(robotState.ki);

  EEPROM.put(robotState.eepromAddress, robotState.uturnAngle);
  robotState.eepromAddress += sizeof(robotState.uturnAngle);

  EEPROM.put(robotState.eepromAddress, robotState.isLeftHandSide);
  robotState.eepromAddress += sizeof(robotState.isLeftHandSide);

  EEPROM.put(robotState.eepromAddress, robotState.frontThreshold);
  robotState.eepromAddress += sizeof(robotState.frontThreshold);

  EEPROM.put(robotState.eepromAddress, robotState.leftThreshold);
  robotState.eepromAddress += sizeof(robotState.leftThreshold);

  EEPROM.put(robotState.eepromAddress, robotState.rightThreshold);
  robotState.eepromAddress += sizeof(robotState.rightThreshold);

  EEPROM.commit();
}

void loadRobotStateFromEEPROM()
{
  robotState.eepromAddress = 0;

  // Load motor parameters
  EEPROM.get(robotState.eepromAddress, robotState.baseSpeed);
  robotState.eepromAddress += sizeof(robotState.baseSpeed);

  EEPROM.get(robotState.eepromAddress, robotState.rightRatio);
  robotState.eepromAddress += sizeof(robotState.rightRatio);

  EEPROM.get(robotState.eepromAddress, robotState.leftRatio);
  robotState.eepromAddress += sizeof(robotState.leftRatio);

  // Load rotation calibration
  EEPROM.get(robotState.eepromAddress, robotState.rotationCalibration);
  robotState.eepromAddress += sizeof(robotState.rotationCalibration);

  // Load PID parameters (Kp, Ki, Kd)
  EEPROM.get(robotState.eepromAddress, robotState.kp);
  robotState.eepromAddress += sizeof(robotState.kp);

  EEPROM.get(robotState.eepromAddress, robotState.kd);
  robotState.eepromAddress += sizeof(robotState.kd);

  EEPROM.get(robotState.eepromAddress, robotState.ki);
  robotState.eepromAddress += sizeof(robotState.ki);

  EEPROM.get(robotState.eepromAddress, robotState.uturnAngle);
  robotState.eepromAddress += sizeof(robotState.uturnAngle);

  EEPROM.get(robotState.eepromAddress, robotState.isLeftHandSide);
  robotState.eepromAddress += sizeof(robotState.isLeftHandSide);

  EEPROM.get(robotState.eepromAddress, robotState.frontThreshold);
  robotState.eepromAddress += sizeof(robotState.frontThreshold);

  EEPROM.get(robotState.eepromAddress, robotState.leftThreshold);
  robotState.eepromAddress += sizeof(robotState.leftThreshold);

  EEPROM.get(robotState.eepromAddress, robotState.rightThreshold);
  robotState.eepromAddress += sizeof(robotState.rightThreshold);

  // Validate PID parameters and set defaults if invalid
  if (robotState.kp < 0 || robotState.kp > 100)
  {
    robotState.kp = 1.0; // Default Kp
  }
  if (robotState.ki < 0 || robotState.ki > 100)
  {
    robotState.ki = 0.0; // Default Ki
  }
  if (robotState.kd < 0 || robotState.kd > 100)
  {
    robotState.kd = 0.0; // Default Kd
  }
}
