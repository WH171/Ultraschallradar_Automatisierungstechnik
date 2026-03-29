/**
 * @file    SoftwareSerialExample.ino
 * @brief   Demonstrates basic communication using the SoftwareSerial library.
 * @details
 * Rundenzaehler fuer die Carrera-Bahn
 * This example sends and receives serial data using the SoftwareSerial library.
 * A software-based serial connection is established between two digital pins.
 * @author Gruppe 2
 * @date  2025-06
 */

#include <SoftwareSerial.h>

#define RX_PIN 10  ///< Digital pin used for receiving data
#define TX_PIN 11  ///< Digital pin used for transmitting data

// Create a SoftwareSerial object named 'mySerial'
SoftwareSerial mySerial(RX_PIN, TX_PIN);

/**
 * @brief Initializes hardware and the software serial port.
 */
void setup() {
  // Start the hardware serial port
  Serial.begin(9600);
  // Start the software serial port
  mySerial.begin(9600);

  // Confirm initialization
  Serial.println("SoftwareSerial communication started.");
  mySerial.println("Hello from SoftwareSerial!");
}

/**
 * @brief Main loop to read data from SoftwareSerial and echo to Serial monitor.
 */
void loop() {
  // Check if data is available on the software serial port
  if (mySerial.available() > 0) {
    char incomingByte = mySerial.read();  ///< Read the received byte
    Serial.print("Received: ");
    Serial.println(incomingByte);
  }

  // Example: Send data every 2 seconds
  static unsigned long lastSent = 0;
  if (millis() - lastSent > 2000) {
    mySerial.println("Ping from SoftwareSerial");
    lastSent = millis();
  }
}
