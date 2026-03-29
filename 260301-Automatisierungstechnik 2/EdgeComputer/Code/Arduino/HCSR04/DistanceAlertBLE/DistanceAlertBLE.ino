/**
 * @file DistanceAlertBLE.ino
 * @brief Ultrasonic distance measurement with BLE notification and LED indicators
 * 
 * @details This sketch uses an HC-SR04 ultrasonic sensor to measure distance and provides
 * visual feedback via LEDs and BLE notifications. The system indicates when an object
 * is within the optimal range (10-30 cm) or outside this range.
 */

#include <NewPing.h>
#include <ArduinoBLE.h>

#define TRIGGER_PIN 9      ///< Arduino pin connected to sensor's trigger pin
#define ECHO_PIN 10        ///< Arduino pin connected to sensor's echo pin
#define MAX_DISTANCE 200   ///< Maximum distance we want to measure (in cm)
#define GREEN_LED 3        ///< Pin for the green LED indicator
#define RED_LED 2          ///< Pin for the red LED indicator

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); ///< NewPing object for distance measurement
BLEService distanceService("180A"); ///< BLE service for distance monitoring
BLEStringCharacteristic distanceChar("2A57", BLERead | BLENotify, 20); ///< BLE characteristic for distance data

/**
 * @brief Initializes hardware and BLE connection
 * 
 * This function sets up the LED pins as outputs and initializes the BLE connection
 * with the specified service and characteristic.
 */
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  if (!BLE.begin()) {
    while (1); // Stop execution if BLE fails to initialize
  }

  BLE.setLocalName("HC-SR04_Monitor");
  BLE.setAdvertisedService(distanceService);
  distanceService.addCharacteristic(distanceChar);
  BLE.addService(distanceService);
  BLE.advertise();
}

/**
 * @brief Main program loop for distance measurement and notification
 * 
 * Continuously measures distance and provides visual/BLE feedback:
 * - Green LED: object within 10-30 cm range
 * - Red LED: object too close (<10 cm) or too far (>30 cm)
 * - BLE notifications with distance status
 */
void loop() {
  BLEDevice central = BLE.central(); // Wait for BLE connection
  
  if (central) {
    int distance = sonar.ping_cm(); // Get distance in cm
    
    if(distance > 0) {
      if(distance >= 10 && distance <= 30) {
        // Object in optimal range
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        distanceChar.writeValue("OK: " + String(distance) + " cm");
      }
      else {
        // Object outside optimal range
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        if(distance < 10) {
          distanceChar.writeValue("ALERT: Too close: " + String(distance) + "cm");
        }
        else {
          distanceChar.writeValue("ALERT: Too far: " + String(distance) + "cm");
        }
      }
    }
    delay(500); // Measurement interval
  }
}
