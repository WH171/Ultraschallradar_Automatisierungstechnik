/**
 * @file bluetoothBeispiel.ino
 * @brief Basic Bluetooth connection indicator using internal LED.
 *
 * This sketch lights up the internal LED (pin 13) when a Bluetooth connection is active.
 * It also sends a confirmation message "OK!" over Bluetooth.
 */

#include <SoftwareSerial.h> ///< Include SoftwareSerial library for Bluetooth communication

/// Create a SoftwareSerial object for Bluetooth communication (RX = pin 2, TX = pin 3)
SoftwareSerial bluetooth(2, 3);

/**
 * @brief Arduino setup function.
 *
 * Sets pin 13 as output and initializes Bluetooth communication.
 */
void setup() {
  pinMode(13, OUTPUT);       ///< Use internal LED (pin 13) as output
  bluetooth.begin(9600);     ///< Start Bluetooth communication at 9600 baud
}

/**
 * @brief Arduino main loop function.
 *
 * Turns the internal LED on when data is available from the Bluetooth module,
 * sends a response message, and prevents message spamming with a short delay.
 */
void loop() {
  if (bluetooth.available()) {     ///< Check if Bluetooth data is available
    digitalWrite(13, HIGH);        ///< Turn on internal LED
    bluetooth.println("OK!");      ///< Send response message
    delay(1000);                   ///< Wait to avoid message flooding
  } else {
    digitalWrite(13, LOW);         ///< Turn off internal LED if no Bluetooth data
  }
}
