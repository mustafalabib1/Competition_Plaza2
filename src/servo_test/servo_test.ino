#include <ESP32Servo.h>

#define SHOULDER_SERVO 13
Servo Shoulder;

void setup() {
  Serial.begin(115200);

  pinMode(SHOULDER_SERVO, OUTPUT);

  Shoulder.attach(SHOULDER_SERVO);  // Attach with min/max pulse widths
  Shoulder.write(90);

  Serial.println("Enter an angle (0-180):");
}

void loop() {
  // Only do something if there is text in the serial buffer
  if (Serial.available() > 0) {
    int angle = Serial.parseInt();  // Reads the whole number (e.g., "90")

    // Simple check to ensure the angle is valid
    if (angle > 0 && angle <= 180) {
      Serial.print("Moving to: ");
      Serial.println(angle);
      Shoulder.write(angle);
    }
  }
}1

