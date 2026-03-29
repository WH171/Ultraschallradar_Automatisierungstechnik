/**
 * @file NewPing.ino
 * @brief Example usage of the library NewPing with ping_timer for asynchronous ultrasonic measurement.
 *
 * @details
 *
 * This file contains a small demonstration of how to use the library NewPing to perform non-blocking
 * distance measurements using the method ping_timer(callback). It uses pin D6 as the trigger and D9 as
 * the echo pin, and outputs the result via Serial. This could be integrated into a larger project such
 * as a Carrera Lap Timer or other sensor-driven applications.
 * @author Wings
 * @date 2025-06
 * 
 */

#include <NewPing.h>

#define TRIGGER_PIN 6  ///< Trigger pin (D6)
#define ECHO_PIN 9     ///< Echo pin (D9)
#define MAX_ENTFERNUNG 200 ///< Maximal distance in cm

NewPing ultraschall(TRIGGER_PIN, ECHO_PIN, MAX_ENTFERNUNG);

unsigned long letzteMessung = 0;
const unsigned long messIntervall = 1000; ///< Measurement interval in ms

/**
 * @brief Callback function for handling measurement results
 * @details Automatically called when measurement completes
 */
void messungFertig() {
  if(ultraschall.check_timer()) {
    unsigned int entfernung = ultraschall.ping_result / US_ROUNDTRIP_CM;
    Serial.print("Aktuelle Entfernung: ");
    Serial.print(entfernung);
    Serial.println(" cm");
  }
}

/**
 * @brief Arduino setup function
 */
void setup() {
  Serial.begin(115200);
  Serial.println("Starte Ultraschallmessung...");
}

/**
 * @brief Main program loop
 */
void loop() {
  if(millis() - letzteMessung >= messIntervall) {
    ultraschall.ping_timer(messungFertig);
    letzteMessung = millis();
  }
}