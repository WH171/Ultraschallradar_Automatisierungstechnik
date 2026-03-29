/**
 * @file NanoRFID_BLE.cpp
 * @brief BLE peripheral example sending a fixed string "hello" to a central device.
 * 
 * This sketch initializes an Arduino BLE peripheral that advertises a custom service
 * and characteristic. When a central device connects (e.g., nRF Connect app), it receives
 * the string "hello" in the characteristic. The RFID reading code is included but inactive.
 * 
 * Hardware:
 * - Arduino Nano 33 BLE or compatible BLE board
 * - MFRC522 RFID module (SPI pins: SS_PIN = 10, RST_PIN = 9)
 * 
 * @date 2025-06-07
 * @author Brocks
 */

#include <ArduinoBLE.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  /**< Slave Select pin for RFID */
#define RST_PIN 9  /**< Reset pin for RFID */

MFRC522 mfrc522(SS_PIN, RST_PIN);  /**< RFID reader object */

// BLE service and characteristic UUIDs
BLEService uidService("180A");                         /**< Custom BLE service UUID */
BLEStringCharacteristic uidChar("2A57", BLERead | BLENotify, 20); /**< BLE characteristic for string data */

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication, SPI, RFID reader, and BLE service.
 * Starts advertising BLE with the local name "NanoRFID".
 * Writes initial value "hello" into the characteristic.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for serial monitor connection on boards like Nano 33 BLE Sense

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Initializing BLE...");

  if (!BLE.begin()) {
    Serial.println("Failed to start BLE!");
    while (1);
  }

  BLE.setLocalName("NanoRFID");
  BLE.setAdvertisedService(uidService);
  uidService.addCharacteristic(uidChar);
  BLE.addService(uidService);

  // Set initial characteristic value to "hello"
  uidChar.writeValue("hello");

  BLE.advertise();

  Serial.println("BLE started and advertising as 'NanoRFID'. Waiting for connections...");
}

/**
 * @brief Arduino main loop.
 * 
 * Waits for a central device to connect. Once connected, sends "hello" once.
 * While connected, RFID reading could be processed (currently inactive).
 * When disconnected, waits for new connections.
 */
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central device: ");
    Serial.println(central.address());

    // Send "hello" to the central device upon connection
    uidChar.writeValue("hello");
    Serial.println("Sent 'hello' to central.");

    while (central.connected()) {
      // RFID reading code could be added here if needed
      delay(100);
    }

    Serial.println("Central device disconnected.");
  }
}
