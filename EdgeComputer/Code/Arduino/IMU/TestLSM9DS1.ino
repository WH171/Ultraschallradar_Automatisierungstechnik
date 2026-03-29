#include <ArduinoBLE.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

// LED pins
const int ledPin1 = 13; // Up/Down
const int ledPin2 = 14; // Left/Right
const int ledPin3 = 15; // Connected and calibrated

void setup() {
  // Initialize LED pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Initialize I2C communication
  Wire.begin();

  // Initialize LSM9DS1 sensor
  if (!imu.begin()) {
    digitalWrite(ledPin3, HIGH); // Light up LED3 in red if sensor initialization fails
    while (1);
  }

  // LED indication: BLE initialized successfully
  digitalWrite(ledPin3, HIGH); // Light up LED3 in red
  delay(500);
  digitalWrite(ledPin3, LOW); // Turn off LED3
}

void loop() {
  // Read accelerometer data
  imu.readAccel();

  // Check the accelerometer data for movement direction
  if (imu.ax > 1 || imu.ax < -1) {
    // Up/Down
    digitalWrite(ledPin1, HIGH); // Turn on LED1 for Up/Down
    digitalWrite(ledPin2, LOW);  // Turn off LED2
  } else if (imu.ay > 1 || imu.ay < -1) {
    // Left/Right
    digitalWrite(ledPin1, LOW);  // Turn off LED1
    digitalWrite(ledPin2, HIGH); // Turn on LED2 for Left/Right
  } else {
    // No significant movement
    digitalWrite(ledPin1, LOW);  // Turn off LED1
    digitalWrite(ledPin2, LOW);  // Turn off LED2
  }
}
