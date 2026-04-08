// #include <Arduino.h>
// #include "motors.h"

// void setup()
// {
//   // Set motor control pins as outputs
//   pinMode(FRONT_LEFT_PWM, OUTPUT);
//   pinMode(FRONT_LEFT_DIR, OUTPUT);

//   Serial.begin(115200);
//   Serial.println("Hello, ESP32!");
//   // put your setup code here, to run once:
// }

// void loop()
// {
//   // put your main code here, to run repeatedly:
//   int speed = 200; // Example speed value
//   speed = Serial.readString().toInt();

//   setMotor(FRONT_LEFT_PWM, FRONT_LEFT_DIR, speed);
//   Serial.println(speed);
//   delay(1000);
//   setMotor(FRONT_LEFT_PWM, FRONT_LEFT_DIR, 0);
//   delay(1000);
// }

// void setMotor(int pwmPin, int dirPin, int speed)
// {
//   // Constraints speed to the valid PWM range (-255 to 255)
//   speed = constrain(speed, -255, 255);
//   digitalWrite(dirPin, speed >= 0 ? HIGH : LOW);
//   analogWrite(pwmPin, speed >= 0 ? 255 - speed : -speed);
// }