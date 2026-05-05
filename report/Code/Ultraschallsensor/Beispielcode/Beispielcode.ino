\begin{verbatim}
// File: Beispielcode.ino
// Standalone script for filtered distance measurement with HC-SR04.
// Uses NewPing library to get a median value from multiple measurements to reduce noise.
// Author: Wilko Hinrichs
// Date: 2026-04-27

#include <NewPing.h>

// Digital pin for trigger signal
#define TRIGGER_PIN  12  

// Digital pin for echo signal
#define ECHO_PIN     11  

// Maximum distance for the sensor (in cm)
#define MAX_DISTANCE 200 

// Initialize NewPing object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200); 
  Serial.println("System gestartet. Beginne Messung...");
}

void loop() {
  // Wait between pings (min. 29ms recommended)
  delay(50); 

  // Get median of 5 measurements (in microseconds)
  unsigned int uS = sonar.ping_median(5);

  Serial.print("Gefilterte Distanz: ");
  
  if (uS == 0) {
    Serial.println("Außerhalb der Reichweite");
  } else {
    // Convert microseconds to centimeters
    Serial.print(uS / US_ROUNDTRIP_CM); 
    Serial.println(" cm");
  }
}
\end{verbatim}