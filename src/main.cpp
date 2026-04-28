// #include "PS4Controller.h"
// #include "motors.h"
// #include "RobotState.h"
// #include "NonBlockingDelay.h"
// #include "AngleController.h"
// #include "RC.h"
// #include "TOF.h"
// #include "ESP32Servo.h"
// #include "MazeSolve.h"

// /* --- SERVO MOTORS --- */
// #define SHOULDER_SERVO 13
// #define ELBOW_SERVO 25
// #define GRIPPER1_SERVO 32
// #define GRIPPER2_SERVO 18
// #define WRIST_SERVO 23

// Servo Shoulder, Elbow, Gripper1, Gripper2, Wrist;

// // servo initial angles
// int shoulderAngle = 90; // Start at a neutral position
// int elbowAngle = 90;
// int gripper1Angle = 90;
// int gripper2Angle = 90;
// int wristAngle = 135;

// // --- Constants for Control ---
// // Driving
// const float JOYSTICK_DEADZONE = 0.1f; // 10% deadzone for joystick
// const float ROTATION_DEADZONE = 0.2f; // 20% deadzone for rotation
// const int MAX_MOTOR_SPEED = 255;

// // Robot Arm
// const int ARM_ANGLE_STEP = 1;
// const int ARM_MIN_ANGLE = 0;
// const int ARM_MAX_ANGLE = 180;

// // Gripper 1 (Standard Servo)
// const int GRIPPER_OPEN_ANGLE = 90;
// const int GRIPPER_CLOSED_ANGLE = 0;

// // Gripper 2 (Continuous Rotation Servo)
// const int GRIPPER_CONTINUOUS_FORWARD = 180; // Spin forward
// const int GRIPPER_CONTINUOUS_BACKWARD = 0;  // Spin backward
// const int GRIPPER_CONTINUOUS_STOP = 90;     // Stop spinning
// // ducration forward and backward
// int direction = 1;
// // --- Global Variables ---
// bool isAutonomousMode = false; // Start in teleop mode

// unsigned long WristDelay = millis();
// unsigned long shoulderDelay = millis();
// unsigned long elbowDelay = millis();

// // --- Function Prototypes ---
// void handleModeSwitching();
// void handleTeleopDrive();
// void handleRobotArmControl();
// void initServos();

// // make task for maze solveing
// TaskHandle_t MazeSolveTask_handle = NULL;

// void MazeSolveTask(void *parameters)
// {
//     for (;;)
//     {
//         solveMaze();
//         // vTaskDelay(5 / portTICK_PERIOD_MS);
//     }
// }

// void setup()
// {
//     // put your setup code here, to run once:
//     Serial.begin(115200);
//     initializeRobotState();
//     MotorsInit();
//     initServos();
//     TofInit();
//     initServos();
//     solveMazeInit();

//     // create maze task
//     xTaskCreate(
//         MazeSolveTask,        // function name
//         "MazeSolveTask",      // Task Name
//         4096,                 // CRITICAL FIX: Increased memory from 1000 to 4096
//         NULL,                 // Task parameters
//         1,                    // Priority
//         &MazeSolveTask_handle // CRITICAL FIX: You MUST use the '&' symbol here!
//     );
//     // Now this will correctly suspend the maze task, instead of freezing setup()!
//     vTaskSuspend(MazeSolveTask_handle);

//     PS4.begin("00:4b:12:3c:5a:82"); // Replace with your ESP32's MAC address
//     Serial.println("Waiting for PS4 controller to connect...");
// }

// void loop()
// {
//     if (PS4.isConnected())
//     {
//         handleModeSwitching();

//         if (isAutonomousMode)
//         {
//             // Autonomous mode logic here (not implemented in this example)
//             // solveMaze();
//         }
//         else
//         {
//             // --- Teleop Control Logic ---
//             handleTeleopDrive();
//             handleRobotArmControl();
//         }
//     }
// }

// void handleModeSwitching()
// {
//     // Use a static variable to track the previous state of the PS button
//     // to detect only the button press event, not holding it down.
//     static bool lastPsButtonState = false;
//     bool currentPsButtonState = PS4.PSButton();

//     if (currentPsButtonState && !lastPsButtonState)
//     {
//         isAutonomousMode = !isAutonomousMode; // Toggle the mode
//         if (isAutonomousMode)
//         {
//             vTaskResume(MazeSolveTask_handle);
//             Serial.println("Switched to Autonomous Mode");
//         }
//         else
//         {
//             vTaskSuspend(MazeSolveTask_handle);
//             Serial.println("Switched to Teleop Mode");
//         }
//     }
//     lastPsButtonState = currentPsButtonState;
// }

// void handleTeleopDrive()
// {
//     // Read and normalize joystick values
//     float y = constrain(-PS4.LStickY() / 127.0f, -1.0f, 1.0f);
//     float rotation = constrain(PS4.RStickX() / 127.0f, -1.0f, 1.0f);

//     // Apply deadzone to prevent drift from slight joystick movements
//     if (fabs(y) < JOYSTICK_DEADZONE)
//         y = 0;

//     if (fabs(rotation) < ROTATION_DEADZONE)
//         rotation = 0;

//     // Apply a non-linear response curve (squaring the input) for finer control at low speeds
//     y = y * fabs(y);
//     rotation = rotation * fabs(rotation);

//     // Convert normalized values to motor speeds and drive the robot
//     driveMecanum(0, y * MAX_MOTOR_SPEED * direction, rotation * MAX_MOTOR_SPEED);
// }
// void handleRobotArmControl()
// {
//     // --- Shoulder Control ---
//     if (PS4.Up())
//         if (millis() - shoulderDelay >= 3)
//         {
//             shoulderAngle += ARM_ANGLE_STEP;
//             shoulderDelay = millis();
//         }
//     if (PS4.Down())
//         if (millis() - shoulderDelay >= 3)
//         {
//             shoulderAngle -= ARM_ANGLE_STEP;
//             shoulderDelay = millis();
//         }
//     // --- Elbow Control ---
//     if (PS4.Triangle())
//         if (millis() - elbowDelay >= 3)
//         {
//             elbowAngle += ARM_ANGLE_STEP;
//             elbowDelay = millis();
//         }
//     if (PS4.Cross())
//         if (millis() - elbowDelay >= 3)
//         {
//             elbowAngle -= ARM_ANGLE_STEP;
//             elbowDelay = millis();
//         }

//     // --- Gripper 1 (Standard Servo) ---
//     if (PS4.Right())
//         gripper1Angle = GRIPPER_OPEN_ANGLE;
//     if (PS4.Left())
//         gripper1Angle = GRIPPER_CLOSED_ANGLE;

//     // --- Wrist Control ---
//     if (PS4.R1())
//         direction = 1;
//     if (PS4.L1())
//         direction = -1;

//     // --- Gripper 2 (Continuous Rotation Servo) ---
//     if (PS4.R2())
//         gripper2Angle = GRIPPER_CONTINUOUS_FORWARD; // Spin forward
//     else if (PS4.L2())
//         gripper2Angle = GRIPPER_CONTINUOUS_BACKWARD; // Spin backward
//     else
//         gripper2Angle = GRIPPER_CONTINUOUS_STOP; // Stop spinning

//     // Constrain all angles to prevent servo damage
//     shoulderAngle = constrain(shoulderAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
//     elbowAngle = constrain(elbowAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
//     wristAngle = constrain(wristAngle, 0, 150);
//     // gripper1Angle is set to specific values, but constraining is good practice
//     gripper1Angle = constrain(gripper1Angle, GRIPPER_CLOSED_ANGLE, GRIPPER_OPEN_ANGLE);
//     // Write angles to servos
//     Shoulder.write(shoulderAngle);
//     Elbow.write(elbowAngle);
//     Gripper1.write(gripper1Angle);
//     Gripper2.write(gripper2Angle);
//     Wrist.write(wristAngle);
//     Serial.println("wristAngle is " + String(wristAngle));
// }

// void initServos()
// {
//     // 1. Allocate hardware timers specifically for the servos
//     // This stops them from fighting with your DC motors!
//     ESP32PWM::allocateTimer(0);
//     ESP32PWM::allocateTimer(1);
//     ESP32PWM::allocateTimer(2);
//     ESP32PWM::allocateTimer(3);

//     // 2. Set the standard frequency for analog servos (50 Hz)
//     Shoulder.setPeriodHertz(50);
//     Elbow.setPeriodHertz(50);
//     Gripper1.setPeriodHertz(50);
//     Gripper2.setPeriodHertz(50);
//     Wrist.setPeriodHertz(50);

//     // 3. Attach servos one by one with a small delay
//     // This prevents all 5 motors from spiking the power supply at once
//     Shoulder.attach(SHOULDER_SERVO, 500, 2400); // 500-2400us is standard
//     Shoulder.write(shoulderAngle);
//     delay(150);

//     Elbow.attach(ELBOW_SERVO, 500, 2400);
//     Elbow.write(elbowAngle);
//     delay(150);

//     Gripper1.attach(GRIPPER1_SERVO, 500, 2400);
//     Gripper1.write(gripper1Angle);
//     delay(150);

//     Gripper2.attach(GRIPPER2_SERVO, 500, 2400);
//     Gripper2.write(gripper2Angle);
//     delay(150);

//     Wrist.attach(WRIST_SERVO, 500, 2400);
//     Wrist.write(wristAngle);
//     delay(150);

//     Serial.println("All Servos Initialized Safely.");
// }