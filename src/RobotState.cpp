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
  robotState.leftCalibrationBase = 0.0;
  robotState.rightCalibrationBase = 0.0;
  robotState.frontCalibrationBase = 0.0;
  robotState.leftCalibrationFactor = 1.0;
  robotState.rightCalibrationFactor = 1.0;
  robotState.frontCalibrationFactor = 1.0;
  robotState.rotationCalibration = 1.0;
  robotState.kp = 1.0;
  robotState.ki = 0.0;
  robotState.kd = 0.0;
  robotState.uturnAngle = 180;
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

  EEPROM.put(robotState.eepromAddress, robotState.leftCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.leftCalibrationBase);

  EEPROM.put(robotState.eepromAddress, robotState.rightCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.rightCalibrationBase);

  EEPROM.put(robotState.eepromAddress, robotState.frontCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.frontCalibrationBase);

  EEPROM.put(robotState.eepromAddress, robotState.leftCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.leftCalibrationFactor);

  EEPROM.put(robotState.eepromAddress, robotState.rightCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.rightCalibrationFactor);

  EEPROM.put(robotState.eepromAddress, robotState.frontCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.frontCalibrationFactor);

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

  // Load calibration parameters
  EEPROM.get(robotState.eepromAddress, robotState.leftCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.leftCalibrationBase);

  EEPROM.get(robotState.eepromAddress, robotState.rightCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.rightCalibrationBase);

  EEPROM.get(robotState.eepromAddress, robotState.frontCalibrationBase);
  robotState.eepromAddress += sizeof(robotState.frontCalibrationBase);

  EEPROM.get(robotState.eepromAddress, robotState.leftCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.leftCalibrationFactor);

  EEPROM.get(robotState.eepromAddress, robotState.rightCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.rightCalibrationFactor);

  EEPROM.get(robotState.eepromAddress, robotState.frontCalibrationFactor);
  robotState.eepromAddress += sizeof(robotState.frontCalibrationFactor);

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
