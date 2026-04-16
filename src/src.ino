
#include "BluetoothSerial.h"
#include "motors.h"
#include "NonBlockingDelay.h"
#include "AngleController.h"
#include "RC.h"
#include "TOF.h"
#include "ESP32Servo.h"
#include "MazeSolve.h"

bool isOg = false;
// Bluetooth communication object
BluetoothSerial SerialBT;

void printHelp();  // Forward declaration

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  initializeRobotState();
  MotorsInit();
  TofInit();
  // Initialize Bluetooth communication
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");

  while (!SerialBT.available()) {
    // Wait for data to be available
  }
  Serial.println("Bluetooth device is ready to pair");

  // Load saved parameters from EEPROM
  loadRobotStateFromEEPROM();

  SerialBT.printf("baseSpeed: %d\n", robotState.baseSpeed);
  SerialBT.printf("rightRatio: %d\n", robotState.rightRatio);
  SerialBT.printf("leftRatio: %d\n", robotState.leftRatio);
  SerialBT.printf("ROTATION_CALIBRATION: %.2f\n", robotState.rotationCalibration);
  SerialBT.printf("Kp: %.2f\n", robotState.kp);
  SerialBT.printf("Kd: %.2f\n", robotState.kd);
  SerialBT.printf("Ki: %.2f\n", robotState.ki);
  SerialBT.printf("uturnAngle: %d\n", robotState.uturnAngle);
  SerialBT.println();
  SerialBT.println("Type 'help' for a list of commands.");
  SerialBT.println("Save");
}

void loop() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();  // Remove trailing spaces and carriage returns

    if (command.length() == 0)
      return;  // Ignore empty lines
    command.toLowerCase();

    // Motor control commands
    if (command.startsWith("basespeed")) {
      int newSpeed = command.substring(9).toInt();
      robotState.baseSpeed = newSpeed;
      SerialBT.printf("baseSpeed: %d\n", robotState.baseSpeed);
    } else if (command.startsWith("rightratio")) {
      int newRatio = command.substring(10).toInt();
      robotState.rightRatio = newRatio;
      SerialBT.printf("rightRatio: %d\n", robotState.rightRatio);
    } else if (command.startsWith("leftratio")) {
      int newRatio = command.substring(9).toInt();
      robotState.leftRatio = newRatio;
      SerialBT.printf("leftRatio: %d\n", robotState.leftRatio);
    } else if (command.startsWith("rotc")) {
      robotState.rotationCalibration = command.substring(4).toFloat();
      SerialBT.printf("ROTATION_CALIBRATION: %.2f\n", robotState.rotationCalibration);
    } else if (command.startsWith("kp")) {
      float newKp = command.substring(2).toFloat();
      robotState.kp = newKp;
      SerialBT.printf("Kp: %.2f\n", robotState.kp);
    } else if (command.startsWith("kd")) {
      float newKd = command.substring(2).toFloat();
      robotState.kd = newKd;
      SerialBT.printf("Kd: %.2f\n", robotState.kd);
    } else if (command.startsWith("ki")) {
      float newKi = command.substring(2).toFloat();
      robotState.ki = newKi;
      SerialBT.printf("Ki: %.2f\n", robotState.ki);
    } else if (command.startsWith("uturnangle")) {
      robotState.uturnAngle = command.substring(10).toInt();
      SerialBT.printf("UTurn Angle: %d\n", robotState.uturnAngle);
    } else if (command.startsWith("islefthand")) {
      String value = command.substring(10);
      value.trim();
      if (value == "true" || value == "1") {
        robotState.isLeftHandSide = true;
        SerialBT.println("Set to Left-Hand Side Maze Solving");
      } else if (value == "false" || value == "0") {
        robotState.isLeftHandSide = false;
        SerialBT.println("Set to Right-Hand Side Maze Solving");
      } else {
        SerialBT.println("Invalid value for isLeftHand. Use 'true' or 'false'.");
      }
    } else if (command.startsWith("ft")) {
      robotState.frontThreshold = command.substring(2).toFloat();
      SerialBT.printf("Front Distance Threshold: %.2f\n", robotState.frontThreshold);
    } else if (command.startsWith("rf")) {
      robotState.rightThreshold = command.substring(2).toFloat();
      SerialBT.printf("Right Distance Threshold: %.2f\n", robotState.rightThreshold);
    } else if (command.startsWith("lf")) {
      robotState.leftThreshold = command.substring(2).toFloat();
      SerialBT.printf("Left Distance Threshold: %.2f\n", robotState.leftThreshold);
    } else if (command.startsWith("save")) {
      saveRobotStateToEEPROM();
      SerialBT.println("Saved");
    } else if (command.startsWith("help")) {
      printHelp();
    } else if (command.startsWith("rc")) {
      SerialBT.println("Entered RC mode. Send 'X' to exit.");
      char RC = 's';
      while (RC != 'X' && RC != 'x') {
        if (SerialBT.available()) {
          RC = SerialBT.read();
          executeCommand(RC);
        }
        delay(10);  // Prevent tight loop from starving the watchdog
      }
      SerialBT.println("Exited RC mode.");
    } else if (command.startsWith("stab")) {
      SerialBT.println("Entered Stabilizer Test mode. Send 'X' to exit.");
      char stabCommand = 's';
      while (stabCommand != 'X' && stabCommand != 'x') {
        if (SerialBT.available()) {
          stabCommand = SerialBT.read();
          stablilizerControlTest(stabCommand);
        }
        delay(10);  // Prevent tight loop from starving the watchdog
      }
      SerialBT.println("Exited Stabilizer Test mode.");
    } else if (command == "maze") {
      SerialBT.println("Solving Maze...");
      solveMazeInit();
      while (SerialBT.readStringUntil('\n') != "stopmaze") {
        solveMaze();
      }
    } else if (command == "right") {
      rotateDegrees(-90);
      while (!isRotationComplete())
        ;
      moveCar(0, 0);
    } else if (command == "left") {
      rotateDegrees(90);
      while (!isRotationComplete())
        ;
      moveCar(0, 0);
    } else if (command == "uturn") {
      UTurn();
      while (!isRotationComplete())
        ;
      moveCar(0, 0);
    } else if (command == "go") {
      while (1) {
        stablilizerControl();
        moveCar(rightMotorSpeed, leftMotorSpeed);
      }
    } else if (command == "stop") {

    } else if (command == "readings" || command == "sensors" || command == "r" || command == "R") {
      SerialBT.println("=== Sensor Readings ===");
      SerialBT.println("Front Distance: " + String(getFrontDistance()));
      SerialBT.println("Right Distance: " + String(getRightDistance()));
      SerialBT.println("Left Distance: " + String(getLeftDistance()));
    } else if (command == "u") {
      // Display current sensor readings and PID parameters
      SerialBT.println("=== Sensor Readings ===");
      SerialBT.println("Front Distance: " + String(getFrontDistance()));
      SerialBT.println("Right Distance: " + String(getRightDistance()));
      SerialBT.println("Left Distance: " + String(getLeftDistance()));
      SerialBT.println();
      SerialBT.println("=== PID Parameters ===");
      SerialBT.printf("Kp: %.2f\n", robotState.kp);
      SerialBT.printf("Ki: %.2f\n", robotState.ki);
      SerialBT.printf("Kd: %.2f\n", robotState.kd);
      SerialBT.println();
      SerialBT.println("=== Motor Speeds ===");
      SerialBT.printf("Base Speed: %d\n", robotState.baseSpeed);
      SerialBT.printf("Left Ratio: %d\n", robotState.leftRatio);
      SerialBT.printf("Right Ratio: %d\n", robotState.rightRatio);
    } else if (command.startsWith("resetpid")) {
      // Reset PID parameters to default values
      robotState.kp = 1.0;
      robotState.ki = 0.0;
      robotState.kd = 0.0;
      SerialBT.println("PID parameters reset to defaults:");
      SerialBT.printf("Kp: %.2f\n", robotState.kp);
      SerialBT.printf("Ki: %.2f\n", robotState.ki);
      SerialBT.printf("Kd: %.2f\n", robotState.kd);
    }
  }
}

void printHelp() {
  SerialBT.println("=== Available Commands ===");
  SerialBT.println();
  SerialBT.println("Motor Control:");
  SerialBT.printf("  basespeed <value>    - Set the base speed (current: %d)\n", robotState.baseSpeed);
  SerialBT.printf("  rightratio <value>   - Set the right ratio (current: %d)\n", robotState.rightRatio);
  SerialBT.printf("  leftratio <value>    - Set the left ratio (current: %d)\n", robotState.leftRatio);
  SerialBT.println();
  SerialBT.println("Sensor Calibration:");
  SerialBT.println();
  SerialBT.println("PID Control:");
  SerialBT.printf("  kp <value>           - Set Kp for PID (current: %.2f)\n", robotState.kp);
  SerialBT.printf("  kd <value>           - Set Kd for PID (current: %.2f)\n", robotState.kd);
  SerialBT.printf("  ki <value>           - Set Ki for PID (current: %.2f)\n", robotState.ki);
  SerialBT.println();
  SerialBT.println("Navigation:");
  SerialBT.printf("  rotc <value>         - Set Rotation Calibration (current: %.2f)\n", robotState.rotationCalibration);
  SerialBT.printf("  uturnangle <value>   - Set U-Turn Angle (current: %d)\n", robotState.uturnAngle);
  SerialBT.println();
  SerialBT.println("Testing Commands:");
  SerialBT.println("  right                - Rotate right 90 degrees");
  SerialBT.println("  left                 - Rotate left 90 degrees");
  SerialBT.println("  uturn                - Perform U-Turn");
  SerialBT.println("  u                    - Display current sensor readings and PID parameters");
  SerialBT.println();
  SerialBT.println("System Commands:");
  SerialBT.println("  save                 - Save settings to EEPROM");
  SerialBT.println("  rc                   - Enter RC mode");
  SerialBT.println("  stab                 - Enter Stabilizer Test mode");
  SerialBT.println("  resetpid             - Reset PID parameters to defaults");
  SerialBT.println(" islefthand <true/false> - Set maze solving side (current: " + String(robotState.isLeftHandSide ? "Left" : "Right") + ")");
  SerialBT.println(" ft <value>            - Set Front Distance Threshold (current: " + String(robotState.frontThreshold) + ")");
  SerialBT.println(" rt <value>            - Set Right Distance Threshold (current: " + String(robotState.rightThreshold) + ")");
  SerialBT.println(" lt <value>            - Set Left Distance Threshold (current: " + String(robotState.leftThreshold) + ")");
  SerialBT.println(" shoulder <angle>      - Set Shoulder servo angle (0-180)");
  SerialBT.println(" elbow <angle>         - Set Elbow servo angle (0-180)");
  SerialBT.println(" gripper1 <angle>      - Set Gripper1 servo angle (0-180)");
  SerialBT.println(" gripper2 <angle>      - Set Gripper2 servo speed (0-180, 90=stop)");
  SerialBT.println(" wrist <angle>         - Set Wrist servo angle (0-180)");
  SerialBT.println("  help                 - Print this help message");
}
