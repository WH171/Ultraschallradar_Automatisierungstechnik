/**
 * @file TestBLEAdver.ino
 * @brief Basic example for starting BLE advertising with ArduinoBLE.
 * 
 * This example initializes the BLE hardware, sets a local device name, 
 * and starts advertising over Bluetooth Low Energy (BLE). It is useful 
 * for testing if the device is visible to BLE scanners (e.g., smartphone apps).
 * 
 * @details
 * - Board: Arduino Nano 33 BLE or compatible
 * - Library: ArduinoBLE
 * - Function: Start BLE advertising with the name "TestBLE"
 * 
 * @version 1.0
 * @date 2025-06-03
 * @author
 * Molsen
 * 
 */

#include <ArduinoBLE.h>

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication and configures BLE.
 * Starts advertising with the local name "TestBLE".
 */
void setup() {
  Serial.begin(9600);

  // Wait for serial connection (required for native USB boards)
  while (!Serial); 

  // Initialize BLE hardware
  if (!BLE.begin()) {
    Serial.println("Error: Failed to initialize BLE.");
    while (1); // Stop if BLE cannot start
  }

  // Set advertised device name
  BLE.setLocalName("TestBLE");

  // Start BLE advertising
  BLE.advertise();

  Serial.println("BLE advertising started.");
}

/**
 * @brief Arduino loop function.
 * 
 * No continuous logic needed. BLE runs in the background.
 */
void loop() {
  // No additional actions required
}
