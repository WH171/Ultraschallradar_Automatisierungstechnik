/**
 * @file SensorSerialReceiver.ino
 * @brief Application for reading sensor data via SoftwareSerial.
 * 
 * This Arduino application sets up a secondary serial interface using the
 * SoftwareSerial library to communicate with an external sensor device.
 * 
 * The program reads sensor data sent via the SoftwareSerial interface (using digital pins 10 and 11),
 * and prints it to the Serial Monitor via the USB connection.
 * 
 * The application ensures that the SoftwareSerial instance is actively listening,
 * and it includes structured Doxygen-style comments for documentation.
 * 
 * @date 2025
 * @version 1.0
 */

#include <SoftwareSerial.h>

/// @brief Create SoftwareSerial instance for sensor communication (RX = 10, TX = 11)
SoftwareSerial sensorSerial(10, 11);

/**
 * @brief Initializes USB serial and SoftwareSerial communication
 */
void setup() {
  Serial.begin(9600);              ///< Start main serial monitor
  sensorSerial.begin(9600);        ///< Start software serial communication
  sensorSerial.listen();           ///< Make sure sensorSerial is listening
  Serial.println("System ready."); ///< Confirm system startup
}

/**
 * @brief Reads sensor data from SoftwareSerial and displays it via USB serial
 */
void loop() {
  // Check if data is available from the sensor
  if (sensorSerial.available()) {
    String sensorData = sensorSerial.readStringUntil('\n'); ///< Read incoming sensor value
    Serial.print("Sensor: ");
    Serial.println(sensorData); ///< Output sensor value
  }

  delay(100); ///< Wait briefly before next check
}

