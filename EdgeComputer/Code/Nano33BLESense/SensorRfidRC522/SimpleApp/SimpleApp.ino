/**
 * @file rtc_example.ino
 * @brief Simple example of using a DS1307 RTC module with an Arduino.
 * 
 * This sketch initializes the DS1307 real-time clock (RTC), sets the time if necessary,
 * and continuously prints the current date and time to the serial monitor.
 */

#include <Wire.h>       ///< Include Wire library for I2C communication.
#include "RTClib.h"     ///< Include Adafruit's RTClib library for easy RTC handling.

// Create an instance of the RTC_DS1307 class to interface with the DS1307 module.
RTC_DS1307 rtc;

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication and the RTC module. If the RTC is not running,
 * the system time at the time of compilation is used to set the clock.
 */
void setup() {
    // Start serial communication at 115200 baud.
    Serial.begin(115200);

    // Attempt to initialize communication with the RTC.
    if (!rtc.begin()) {
        Serial.println("RTC not found!");
        while (1); // Halt execution if RTC is not found.
    }

    // Check if the RTC is running. If not, set the time to the compilation time.
    if (!rtc.isrunning()) {
        Serial.println("RTC is not running, setting the current time.");
        // Set the RTC time to the time at which the sketch was compiled.
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

/**
 * @brief Arduino main loop function.
 * 
 * Continuously reads the current time from the RTC and prints it in
 * DD.MM.YYYY HH:MM:SS format to the serial monitor every second.
 */
void loop() {
    // Get the current date and time from the RTC.
    DateTime now = rtc.now();

    // Print the current date in DD.MM.YYYY format.
    Serial.print(now.day(), DEC);
    Serial.print('.');
    Serial.print(now.month(), DEC);
    Serial.print('.');
    Serial.print(now.year(), DEC);
    Serial.print(" ");

    // Print the current time in HH:MM:SS format.
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);

    // Wait for one second before the next update.
    delay(1000);
}
