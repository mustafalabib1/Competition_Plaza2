#include "tof.h"
#include <Wire.h>
#include <VL53L1X.h>

// XSHUT pins
#define LEFT_XSHUT 13
#define FRONT_XSHUT 14
#define RIGHT_XSHUT 27

// I2C pins
#define SDA_PIN 21
#define SCL_PIN 22

// Create sensor objects
VL53L1X sensorLeft;
VL53L1X sensorFront;
VL53L1X sensorRight;

// Distances
float LeftDistance, RightDistance, FrontDistance;

void TofInit()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    Serial.println("\n--- Starting ToF Boot Sequence ---");

    // 1. HARDWARE SHUTDOWN (Active LOW)
    // Pull all XSHUT pins LOW to force all sensors to sleep
    pinMode(LEFT_XSHUT, OUTPUT);
    pinMode(FRONT_XSHUT, OUTPUT);
    pinMode(RIGHT_XSHUT, OUTPUT);
    digitalWrite(LEFT_XSHUT, LOW);
    digitalWrite(FRONT_XSHUT, LOW);
    digitalWrite(RIGHT_XSHUT, LOW);
    delay(150); // Crucial: Give capacitors time to drain completely

    /* --- Activate LEFT sensor --- */
    Serial.println("Waking up LEFT sensor...");
    digitalWrite(LEFT_XSHUT, HIGH); // Pull HIGH to wake up
    delay(50);                      // Give it time to boot (Datasheet says max 1.2ms, but 50ms is safe)

    sensorLeft.setTimeout(500);
    if (!sensorLeft.init())
    {
        Serial.println("ERROR: Failed to initialize LEFT sensor!");
    }
    else
    {
        sensorLeft.setAddress(0x30);
        sensorLeft.setDistanceMode(VL53L1X::Short);   // Configure distance mode
        sensorLeft.setMeasurementTimingBudget(50000); // 50ms timing budget
        sensorLeft.startContinuous(50);
        Serial.println("LEFT sensor ready at 0x30.");
    }

    /* --- Activate FRONT sensor --- */
    Serial.println("Waking up FRONT sensor...");
    digitalWrite(FRONT_XSHUT, HIGH);
    delay(50);

    sensorFront.setTimeout(500);
    if (!sensorFront.init())
    {
        Serial.println("ERROR: Failed to initialize FRONT sensor!");
    }
    else
    {
        sensorFront.setAddress(0x31);
        sensorFront.setDistanceMode(VL53L1X::Short);
        sensorFront.setMeasurementTimingBudget(50000);
        sensorFront.startContinuous(50);
        Serial.println("FRONT sensor ready at 0x31.");
    }

    /* --- Activate RIGHT sensor --- */
    Serial.println("Waking up RIGHT sensor...");
    digitalWrite(RIGHT_XSHUT, HIGH);
    delay(50);

    sensorRight.setTimeout(500);
    if (!sensorRight.init())
    {
        Serial.println("ERROR: Failed to initialize RIGHT sensor!");
    }
    else
    {
        sensorRight.setAddress(0x32);
        sensorRight.setDistanceMode(VL53L1X::Short);
        sensorRight.setMeasurementTimingBudget(50000);
        sensorRight.startContinuous(50);
        Serial.println("RIGHT sensor ready at 0x32.");
    }
}

void PowerOffTofSensors()
{
    sensorLeft.stopContinuous();
    sensorFront.stopContinuous();
    sensorRight.stopContinuous();

    // Pull LOW to physically force all sensors to sleep and save power
    digitalWrite(LEFT_XSHUT, LOW);
    digitalWrite(FRONT_XSHUT, LOW);
    digitalWrite(RIGHT_XSHUT, LOW);

    Serial.println("First Stage Complete: ToF Sensors Powered Down.");
}

float getFrontDistance()
{
    // Use .read() for the VL53L1X library
    uint16_t rawDist = sensorFront.read();

    // Filter out timeouts and out-of-range values
    if (sensorFront.timeoutOccurred() || rawDist > 8000)
    {
        return 0; // Return 0 so the robot doesn't freak out
    }

    FrontDistance = rawDist / 10.0;
    return (FrontDistance > robotState.frontCalibrationBase) ? (FrontDistance - robotState.frontCalibrationBase) * 5.0 / robotState.frontCalibrationFactor : 0;
}

float getRightDistance()
{
    uint16_t rawDist = sensorRight.read();
    if (sensorRight.timeoutOccurred() || rawDist > 8000)
    {
        return 0;
    }

    RightDistance = rawDist / 10.0;
    return (RightDistance > robotState.rightCalibrationBase) ? (RightDistance - robotState.rightCalibrationBase) * 5.0 / robotState.rightCalibrationFactor : 0;
}

float getLeftDistance()
{
    uint16_t rawDist = sensorLeft.read();
    if (sensorLeft.timeoutOccurred() || rawDist > 8000)
    {
        return 0;
    }

    LeftDistance = rawDist / 10.0;
    return (LeftDistance > robotState.leftCalibrationBase) ? (LeftDistance - robotState.leftCalibrationBase) * 5.0 / robotState.leftCalibrationFactor : 0;
}