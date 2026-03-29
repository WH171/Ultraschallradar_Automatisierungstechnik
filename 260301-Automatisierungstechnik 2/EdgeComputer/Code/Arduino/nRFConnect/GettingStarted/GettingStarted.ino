/**
 * @file einrichtungNRF.ino
 * @brief Minimal BLE advertising example for nRF Connect visibility.
 *
 * This Arduino sketch enables BLE advertising so the device appears in the
 * nRF Connect mobile app. No services or characteristics are defined.
 */

#include <ArduinoBLE.h>  ///< Include the ArduinoBLE library

/**
 * @brief Arduino setup function.
 *
 * Initializes BLE and starts advertising with a custom device name.
 * The device will be visible in the nRF Connect app under the name "SimpleArduino".
 */
void setup() {
  Serial.begin(9600);

  // Initialize BLE hardware
  if (!BLE.begin()) {
    Serial.println("BLE failed to initialize!");
    while (1); // Halt execution if BLE cannot start
  }

  // Set the advertised device name
  BLE.setLocalName("Mein Arduino");

  // Start BLE advertising without any services or characteristics
  BLE.advertise();

  Serial.println("BLE advertising... You can now see the device in nRF Connect.");
}

/**
 * @brief Arduino main loop.
 *
 * No actions are performed here; BLE continues advertising in the background.
 */
void loop() {
  // No operation needed
}
