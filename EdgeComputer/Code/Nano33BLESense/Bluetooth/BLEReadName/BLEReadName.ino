/**
 * @file TestBLEReadName.ino
 * @brief BLE Advertising and Central Device Connection Logger
 * 
 * This sketch initializes BLE on a compatible Arduino board, 
 * starts advertising with a local device name, and detects when 
 * a central device connects. It logs the central devices BLE address 
 * and reports when the connection is closed.
 * 
 * @details
 * - Board: Arduino Nano 33 BLE or compatible
 * - Library: ArduinoBLE
 * - Functionality:
 *   - Start BLE advertising with the name "TestBLE"
 *   - Detect and log connections from central BLE devices
 * 
 * @version 1.0
 * @date 2025-06-03
 * @author
 * Molsen
 * 
 */

#include <ArduinoBLE.h>

/// @brief Global handle for the connected central device
BLEDevice centralDevice;

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication and BLE, sets a local name,
 * and starts BLE advertising.
 */
void setup() {
  Serial.begin(9600);

  // Wait for serial monitor connection (USB only)
  while (!Serial);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Error: Failed to initialize BLE.");
    while (1); // Stop if initialization fails
  }

  // Set the BLE device name for advertisement
  BLE.setLocalName("TestBLE");

  // Begin BLE advertising
  BLE.advertise();

  Serial.println("BLE advertising started.");
}

/**
 * @brief Arduino loop function.
 * 
 * Monitors for connections from central BLE devices.
 * Logs connection and disconnection events to the serial monitor.
 */
void loop() {
  // Check if a central device is connected
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Device connected: ");
    Serial.println(central.address());

    // While connected, hold the loop
    while (central.connected()) {
      delay(100);
    }

    Serial.println("Connection lost.");
  }
}

