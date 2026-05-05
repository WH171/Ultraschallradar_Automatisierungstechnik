\begin{verbatim}
// File: Beispielcode2.ino
// Standalone script for filtered distance measurement with HC-SR04.
// Uses NewPing library to get a median value from multiple measurements to reduce noise.
// Author: Wilko Hinrichs
// Date: 2026-04-27

const int TRIG_PIN = 12; // Pin for trigger signal
const int ECHO_PIN = 11; // Pin for echo signal

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // 10 microseconds HIGH pulse (ends with falling edge)
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  
  // pulseIn measures the length of the ECHO pulse
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance (travel time in us / 58)
  float distance = duration / 58.0; 
  
  Serial.print("Gemessene Distanz: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Keep interval (> 20ms)
  delay(50); 
}
\end{verbatim}