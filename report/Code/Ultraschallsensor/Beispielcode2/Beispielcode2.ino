/**
 * @file    HCSR04_Native_Example.ino
 * @brief   Native Distanzmessung ohne externe Bibliotheken.
 */

const int TRIG_PIN = 12; 
const int ECHO_PIN = 11; 

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // 10 Mikrosekunden HIGH-Impuls (Endet mit fallender Flanke)
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);
  
  /** * @brief pulseIn misst die Länge des ECHO-Impulses. */
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Berechnung der Distanz (Laufzeit in us / 58)
  float distance = duration / 58.0; 
  
  Serial.print("Gemessene Distanz: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(50); // Intervall einhalten (> 20ms)
}