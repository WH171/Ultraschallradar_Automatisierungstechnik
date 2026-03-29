/**
 * @file bluetoothAnwendung.ino
 * @brief Bluetooth-controlled relay for switching a floor lamp on and off.
 *
 * This sketch uses a Bluetooth module connected via SoftwareSerial to control a relay.
 * Commands 'E' and 'A' turn the relay ON and OFF, respectively.
 */

#include <SoftwareSerial.h> ///< Include SoftwareSerial library for Bluetooth communication

#define RELAY_PIN 7 ///< Arduino pin connected to the relay module

/// Create a SoftwareSerial object for Bluetooth communication (RX = pin 2, TX = pin 3)
SoftwareSerial bluetooth(2, 3);

/**
 * @brief Arduino setup function.
 *
 * Initializes the relay pin and starts the Bluetooth serial communication.
 * Sends an initial message to the Bluetooth device to indicate readiness.
 */
void setup() {
  pinMode(RELAY_PIN, OUTPUT);       ///< Set the relay pin as output
  digitalWrite(RELAY_PIN, LOW);     ///< Start with the relay turned OFF
  bluetooth.begin(9600);            ///< Start Bluetooth communication at 9600 baud
  bluetooth.println("Floor lamp ready! Send 'E' to turn ON, 'A' to turn OFF.");
}

/**
 * @brief Arduino main loop function.
 *
 * Continuously checks for incoming Bluetooth commands and toggles the relay accordingly.
 */
void loop() {
  if (bluetooth.available()) {
    char cmd = bluetooth.read(); ///< Read the incoming command character

    if (cmd == 'E') {
      digitalWrite(RELAY_PIN, HIGH); ///< Turn the relay ON
      bluetooth.println("Floor lamp ON");
    } 
    else if (cmd == 'A') {
      digitalWrite(RELAY_PIN, LOW); ///< Turn the relay OFF
      bluetooth.println("Floor lamp OFF");
    }
  }
}
