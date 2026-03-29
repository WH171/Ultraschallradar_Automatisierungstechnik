/**
 * @file testBLE.ino
 * @brief Simple BLE test for Arduino Nano 33 BLE Sense using nRF Connect app.
 * 
 * This sketch initializes the built-in BLE module (NINA-B306)
 * and makes the device discoverable. You can connect with
 * the nRF Connect app to test BLE functionality.
 */

#include <ArduinoBLE.h>  // Include BLE library

/// Name of the BLE device shown in nRF Connect
#define DEVICE_NAME "Nano33 BLE Test"

/// BLE Service (custom or UART-like service)
BLEService testService("180C");  // 0x180C = User-defined service (can be changed)

/// A simple characteristic to test reading/writing
BLEStringCharacteristic testCharacteristic("2A56", BLERead | BLEWrite, 20);

/**
 * @brief Arduino setup function.
 * Initializes serial communication and starts BLE advertising.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set device name and service
  BLE.setLocalName(DEVICE_NAME);
  BLE.setAdvertisedService(testService);

  // Add characteristic to service
  testService.addCharacteristic(testCharacteristic);
  BLE.addService(testService);

  // Set initial value
  testCharacteristic.setValue("BLE funktioniert!");

  // Start advertising
  BLE.advertise();
  Serial.println("BLE device is now advertising...");
}

/**
 * @brief Arduino loop function.
 * Accepts connections and prints info when connected.
 */
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Optional: read value if changed
      if (testCharacteristic.written()) {
        Serial.print("Received from app: ");
        Serial.println(testCharacteristic.value());
      }
    }

    Serial.println("Disconnected.");
  }
}
