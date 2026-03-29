// Test of the Power LED
//
// File: TestLEDPowerBattery.ino
//
// Check the battery state.

#include <Arduino.h>

#define LED_PWR 25 // Define the pin for the power LED

// Battery measurement pin
const int BATTERY_PIN = A0;
// Reference voltage for 3.3V (adjust if using external power)
const float REFERENCE_VOLTAGE = 3.3;

const int LED_PWR = 25;

void setup() {
    // Set LED pin as output
    pinMode(LED_PWR, OUTPUT);
    // Initialize serial communication for debugging
    Serial.begin(9600);
}

// Input:
// SendMessages false - no messages
// true - with messages
// Return: 0 - okay // 1 - < 20%
int BatteryState(bool SendMessages) {
    int ret;

    // Read battery voltage from analog pin
    float rawVoltage = analogRead(BATTERY_PIN) * (REFERENCE_VOLTAGE / 1023.0);
    // Calculate percentage based on reference voltage (adjust based on battery specs)
    float batteryPercentage = (rawVoltage / 4.2) * 100.0;
    
    if (SendMessages) {
        // Print battery voltage and percentage (for debugging)
        Serial.print("Battery voltage: ");
        Serial.print(rawVoltage);
        Serial.println(" V");
        Serial.print("Battery percentage: ");
        Serial.print(batteryPercentage);
        Serial.println("%");
    }

    // Optional: blink LED based on battery level (adjust thresholds)
    if (batteryPercentage < 20) {
        digitalWrite(LED_PWR, HIGH);
        delay(500);
        digitalWrite(LED_PWR, LOW);
        delay(500);
        ret = 1;
    } else {
        digitalWrite(LED_PWR, HIGH);
        ret = 0;
    }

    return ret;
}

void loop() {
    BatteryState(false);
    // Delay between measurements
    delay(5000);
}
