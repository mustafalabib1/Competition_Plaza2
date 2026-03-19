#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X tofSensor;

void setup() {
  Serial.begin(9600);
    Wire.begin(21, 22);;

  if(tofSensor.init() != true){
    Serial.println("Could not initialize ToF sensor.");
    // Handle that error
  }
}

void loop() {
  Serial.print(tofSensor.readRangeSingleMillimeters());
  Serial.println(" mm");

  delay(500);
}