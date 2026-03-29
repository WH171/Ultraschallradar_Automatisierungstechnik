#include <Wire.h>

/**
 * @file Wire.h-Example.ino
 * @brief Initializes the I2C bus and sends a single command to a device at address 0x3C.
 * 
 * This example demonstrates how to start I2C communication and transmit a single byte
 * (e.g., a control command) to a connected I2C device.
 */

/**
 * @brief Initializes the I2C bus and sends a single command to a device at address 0x3C.
 * 
 * Starts I2C communication, transmits one byte (e.g., a control byte),
 * and then ends the transmission.
 */
void setup() {
  Wire.begin();                   ///< Start I2C communication
  Wire.beginTransmission(0x3C);  ///< Begin transmission to device at address 0x3C
  Wire.write(0x00);              ///< Send a single byte (e.g., control command)
  Wire.endTransmission();        ///< End the transmission
}