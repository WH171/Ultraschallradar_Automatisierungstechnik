\begin{verbatim}
// File: EinparkhilfeCode.ino
// Application example: Automated parking assistant.
// System shows distance via LED and warns acoustically on critical approach.
// Author: Wilko Hinrichs
// Date: 2026-04-28

#include <NewPing.h>

// Pin configuration
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200  // Maximum scan range in cm
#define LED_PIN      3    // Yellow warning LED
#define BUZZER_PIN   4    // Acoustic buzzer

// Initialize sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Keep measurement interval
  delay(50);                     
  
  // Measure distance in cm
  unsigned int distance = sonar.ping_cm(); 

  Serial.print("Abstand: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Logic check based on distance thresholds
  if (distance > 0 && distance < 10) {
    // Critical area: LED and buzzer active
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } 
  else if (distance >= 10 && distance < 50) {
    // Warning area: Only LED active
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  else {
    // Safe area: All off
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}
\end{verbatim}