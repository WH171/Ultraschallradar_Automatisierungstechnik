/**
 * @file HCSR04Calibration.ino
 * @brief HC-SR04 ultrasonic sensor calibration tool
 * @details 
 * Rundenzaehler fuer die Carrera-Bahn
 * Measures known distance (20cm) and calculates calibration factor
 * @author Gruppe 2
 * @date 2025-06
 */

#include <NewPing.h>

#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define KNOWN_DISTANCE 20.0 // 20 cm reference distance
#define SAMPLES 25          // Number of calibration samples

NewPing sonar(TRIGGER_PIN, ECHO_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("HC-SR04 Calibration");
  Serial.println("Place 'Rundenzaehler' over the Racingtrack");
}

void loop() {
  float sum = 0;
  
  // Take multiple samples
  for(int i = 0; i < SAMPLES; i++) {
    delay(50);
    float distance = sonar.ping_cm();
    if(distance > 0) {
      sum += distance;
    }
  }

  // Calculate calibration factor
  float measured = sum / SAMPLES;
  float factor = KNOWN_DISTANCE / measured;
  
  Serial.print("Measured: ");
  Serial.print(measured, 2);
  Serial.print(" cm | Calibration factor: ");
  Serial.println(factor, 2);
  
  delay(1000);
}
