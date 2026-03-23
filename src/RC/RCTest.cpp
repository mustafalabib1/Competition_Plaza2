
#include "BluetoothSerial.h"
#include "motors/motors.h"
#include "RobotState.h"
#include "NonBlockingDelay.h"
#include "AngleController/AngleController.h"
#include "RC/RC.h"

// Bluetooth communication object
BluetoothSerial SerialBT;

void setup()
{
    // Initialize serial communication for debugging
    Serial.begin(115200);
    initializeRobotState();
    MotorsInit();

    // Initialize Bluetooth communication
    SerialBT.begin("ESP32test");
    Serial.println("The device started, now you can pair it with bluetooth!");

    while (!SerialBT.available())
    {
        // Wait for data to be available
    }
    Serial.println("Bluetooth device is ready to pair");
}

void loop()
{
    if (SerialBT.available())
    {
        char command = SerialBT.read();
        executeCommand(command);
    }
}