#include "tof.h"
#include <Wire.h>
#include <VL53L0X.h>

// XSHUT pins
#define LEFT_XSHUT 13
#define FRONT_XSHUT 14
#define RIGHT_XSHUT 27

// I2C pins
#define SDA_PIN 21
#define SCL_PIN 22

// Create sensor objects
VL53L0X sensorLeft;
VL53L0X sensorFront;
VL53L0X sensorRight;

// Distances
float LeftDistance, RightDistance, FrontDistance;

void TofInit()
{
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(LEFT_XSHUT, OUTPUT);
  pinMode(FRONT_XSHUT, OUTPUT);
  pinMode(RIGHT_XSHUT, OUTPUT);

  // Turn OFF all sensors
  digitalWrite(LEFT_XSHUT, LOW);
  digitalWrite(FRONT_XSHUT, LOW);
  digitalWrite(RIGHT_XSHUT, LOW);
  delay(10);

  // Activate LEFT sensor
  digitalWrite(LEFT_XSHUT, HIGH);
  delay(10);
  sensorLeft.init();
  sensorLeft.setAddress(0x30);

  // Activate FRONT sensor
  digitalWrite(FRONT_XSHUT, HIGH);
  delay(10);
  sensorFront.init();
  sensorFront.setAddress(0x31);

  // Activate RIGHT sensor
  digitalWrite(RIGHT_XSHUT, HIGH);
  delay(10);
  sensorRight.init();
  sensorRight.setAddress(0x32);

  // Start continuous mode
  sensorLeft.startContinuous();
  sensorFront.startContinuous();
  sensorRight.startContinuous();
}
void PowerOffTofSensors()
{
  // 1. Tell the sensors to stop firing cleanly
  sensorLeft.stopContinuous();
  sensorFront.stopContinuous();
  sensorRight.stopContinuous();

  // 2. Physically cut their power using XSHUT
  digitalWrite(LEFT_XSHUT, LOW);
  digitalWrite(FRONT_XSHUT, LOW);
  digitalWrite(RIGHT_XSHUT, LOW);

  Serial.println("First Stage Complete: ToF Sensors Powered Down.");
}
float getFrontDistance()
{
  FrontDistance = sensorFront.readRangeContinuousMillimeters() / 10.0;
  return (FrontDistance > robotState.frontCalibrationBase) ? (FrontDistance - robotState.frontCalibrationBase) * 5.0 / robotState.frontCalibrationFactor : 0;
  // return sensorFront.readRangeContinuousMillimeters() ;
}

float getRightDistance()
{
  RightDistance = sensorRight.readRangeContinuousMillimeters() / 10.0;
  return (RightDistance > robotState.rightCalibrationBase) ? (RightDistance - robotState.rightCalibrationBase) * 5.0 / robotState.rightCalibrationFactor : 0;
  // return sensorRight.readRangeContinuousMillimeters() ;
}

float getLeftDistance()
{
  LeftDistance = sensorLeft.readRangeContinuousMillimeters() / 10.0;
  return (LeftDistance > robotState.leftCalibrationBase) ? (LeftDistance - robotState.leftCalibrationBase) * 5.0 / robotState.leftCalibrationFactor : 0;
  // return sensorLeft.readRangeContinuousMillimeters() ;
}
