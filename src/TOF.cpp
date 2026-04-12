#include "tof.h"
#include <Wire.h>
#include <VL53L1X.h>

// XSHUT pins
// NOTE: LEFT_XSHUT (Pin 13) is REMOVED!
// Physically wire the Left Sensor's XSHUT pin directly to 3.3V.
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
double FrontDistance = 0;
double RightDistance = 0;
double LeftDistance = 0;

void TofInit()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    Serial.println("\n--- Starting ToF Boot Sequence ---");

    // 1. HARDWARE SHUTDOWN FOR FRONT & RIGHT
    // We only pull Front and Right LOW because Left is hardwired to stay awake
    pinMode(FRONT_XSHUT, OUTPUT);
    pinMode(RIGHT_XSHUT, OUTPUT);
    digitalWrite(FRONT_XSHUT, LOW);
    digitalWrite(RIGHT_XSHUT, LOW);
    delay(150); // Crucial: Give capacitors time to drain completely

    /* --- Activate LEFT sensor (Always Awake) --- */
    Serial.println("Initializing hardwired LEFT sensor...");
    sensorLeft.setTimeout(500);

    // Because it never powers down, it might still be at 0x30 from a previous run!
    // We use a "Smart Init" to check both 0x29 and 0x30.
    if (!sensorLeft.init())
    {
        Serial.println("LEFT not at 0x29. Checking 0x30 (Soft Reset recovery)...");
        sensorLeft.setAddress(0x30);

        if (!sensorLeft.init())
        {
            Serial.println("ERROR: Failed to initialize LEFT sensor entirely!");
        }
        else
        {
            Serial.println("LEFT sensor recovered safely at 0x30.");
        }
    }
    else
    {
        // It was at 0x29, so we safely move it to 0x30
        sensorLeft.setAddress(0x30);
        Serial.println("LEFT sensor moved to 0x30.");
    }

    // Configure LEFT
    sensorLeft.setDistanceMode(VL53L1X::Short);
    sensorLeft.setMeasurementTimingBudget(50000);
    sensorLeft.startContinuous(50);

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

    // Pull LOW to physically force Front and Right to sleep
    // (Left will stay awake, but stopped reading to save logic power)
    digitalWrite(FRONT_XSHUT, LOW);
    digitalWrite(RIGHT_XSHUT, LOW);

    Serial.println("First Stage Complete: ToF Sensors Powered Down.");
}

float getFrontDistance()
{
    uint16_t rawDist = sensorFront.read();
    if (sensorFront.timeoutOccurred() || rawDist > 8000)
        return 0;

    return rawDist;
}

float getRightDistance()
{
    uint16_t rawDist = sensorRight.read();
    if (sensorRight.timeoutOccurred() || rawDist > 8000)
    {
        return 0;
    }
    return rawDist;
}

float getLeftDistance()
{
    uint16_t rawDist = sensorLeft.read();
    if (sensorLeft.timeoutOccurred() || rawDist > 8000)
    {
        return 0;
    }
    return rawDist;
}