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

// Helper function to initialize a sensor and handle Soft Reset recovery
bool initTofSensor(VL53L1X &sensor, uint8_t targetAddress, const char *name)
{
    sensor.setTimeout(500);

    // 1. Try to initialize at default address (0x29)
    if (!sensor.init())
    {
        Serial.print(name);
        Serial.print(" not at 0x29. Checking 0x");
        Serial.print(targetAddress, HEX);
        Serial.println(" (Soft Reset recovery)...");

        // Update the sensor's internal address object to the target
        // (Even if writing to 0x29 fails, it updates internal address to targetAddress)
        sensor.setAddress(targetAddress);

        // 2. Try to initialize at the new target address
        if (!sensor.init())
        {
            Serial.print("ERROR: Failed to initialize ");
            Serial.print(name);
            Serial.println(" sensor entirely!");
            return false;
        }
        else
        {
            Serial.print(name);
            Serial.print(" sensor recovered safely at 0x");
            Serial.println(targetAddress, HEX);
        }
    }
    else
    {
        // 3. Sensor responded at 0x29, so safely move it to the target address
        sensor.setAddress(targetAddress);
        Serial.print(name);
        Serial.print(" sensor moved to 0x");
        Serial.println(targetAddress, HEX);
    }

    // Configure the sensor
    sensor.setDistanceMode(VL53L1X::Short);
    sensor.setMeasurementTimingBudget(50000);
    sensor.startContinuous(50);

    return true;
}

void TofInit()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

<<<<<<< HEAD
<<<<<<< HEAD
    // 2. Set the speed to 400kHz (Fast Mode)
    Wire.setClock(400000);

=======
>>>>>>> parent of 2103939 (Refactor maze solving logic and improve sensor reading handling)
=======
>>>>>>> parent of 2103939 (Refactor maze solving logic and improve sensor reading handling)
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
    initTofSensor(sensorLeft, 0x30, "LEFT");

    /* --- Activate FRONT sensor --- */
    Serial.println("Waking up FRONT sensor...");
    digitalWrite(FRONT_XSHUT, HIGH);
    delay(50);
    initTofSensor(sensorFront, 0x31, "FRONT");

    /* --- Activate RIGHT sensor --- */
    Serial.println("Waking up RIGHT sensor...");
    digitalWrite(RIGHT_XSHUT, HIGH);
    delay(50);
    initTofSensor(sensorRight, 0x32, "RIGHT");

    Serial.println("\n--- ToF Boot Sequence Complete ---");
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