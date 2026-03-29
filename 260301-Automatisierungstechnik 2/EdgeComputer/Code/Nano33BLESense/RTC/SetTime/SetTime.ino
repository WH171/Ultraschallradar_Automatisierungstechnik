/**
 * @file rtc_time_set.ino
 * @brief Example for initializing and reading from a DS3231 RTC using Arduino Nano 33 BLE Sense.
 *
 * @details
 * This sketch initializes the DS3231 Real-Time Clock (RTC) module and sets a fixed
 * startup time if the RTC has lost power. It then continuously prints the current
 * date and time to the Serial Monitor.
 *
 * Components used:
 * - \PYTHON{Wire.h} (I2C communication, included with Arduino IDE)
 * - \PYTHON{RTClib.h} (Adafruit RTC library, install via Library Manager)
 * - DS3231 Real-Time Clock (RTC) Module
 * - Arduino Nano 33 BLE Sense
 *
 * I2C connections (for Arduino Nano 33 BLE Sense):
 * - SDA  A4
 * - SCL  A5
 * - VCC  3.3V
 * - GND  GND
 *
 * @warning Ensure your DS3231 module is compatible with 3.3V logic levels to avoid hardware damage.
 *
 * @author Brocks
 * @date 2025-06-04
 */

#include <Wire.h>       ///< \PYTHON{Wire.h} - I2C communication (built-in)
#include "RTClib.h"     ///< \PYTHON{RTClib.h} - DS3231 RTC library by Adafruit

RTC_DS3231 rtc;         ///< Instance of the DS3231 RTC

/**
 * @brief Initializes the serial port and RTC. If RTC lost power, it sets the time to 2000-01-01 00:06:04.
 */
void setup() {
  Serial.begin(115200);
  while (!Serial);              // Wait for USB serial connection on Nano 33 BLE Sense

  Wire.begin();                 // Initialize I2C

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Please check connections.");
    while (1);                  // Halt if RTC is not found
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power. Setting fixed startup time.");
    rtc.adjust(DateTime(2000, 1, 1, 0, 6, 4));  // Set time only once after power loss
  }
}

/**
 * @brief Reads the current time from the RTC and prints it every second.
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

  delay(1000);  // Wait one second
}
