/**
 * @file rtc_time_set.ino
 * @brief Example for initializing and reading from a DS3231 RTC using Arduino Nano 33 BLE Sense.
 *
 * @details
 * This sketch sets the time on a DS3231 RTC to a fixed value on startup and continuously
 * prints the current time to the Serial Monitor.
 *
 * Components used:
 * - \PYTHON{Wire.h} (I2C communication, included with Arduino IDE)
 * - \PYTHON{RTClib.h} (Adafruit RTC library, install via Library Manager)
 * - DS3231 Real-Time Clock (RTC) Module
 * - Arduino Nano 33 BLE Sense
 *
 * I2C Connections for Nano 33 BLE Sense:
 * - SDA   A4
 * - SCL   A5
 * - VCC   3.3V
 * - GND   GND
 *
 * @warning Ensure the DS3231 module supports 3.3V logic levels.
 * 
 * @author Brocks und Bauer
 * @date 2025-06-04
 */

#include <Wire.h>       ///< I2C library (built-in)
#include "RTClib.h"     ///< RTC library for DS3231 (install via Library Manager)

RTC_DS3231 rtc;         ///< RTC instance

/**
 * @brief Initializes serial and RTC communication and sets a fixed time on the DS3231.
 */
void setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for serial connection

  Wire.begin();     // Start I2C communication

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time...");
    rtc.adjust(DateTime(2000, 1, 1, 0, 6, 4));  // Fixed startup time
  }
}

/**
 * @brief Continuously reads and prints the current time from the RTC.
 */
void loop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
