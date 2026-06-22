/**
 * @file Ultraschallradar.ino
 * @brief Optimierte Radarsteuerung mit neuer LED-Logik und 120°-Scan (30°-150°)
 */

#include <Servo.h>

const int PIN_LED_GREEN = 2;   
const int PIN_LED_RED   = 3;   
const int PIN_TRIG      = 4;   
const int PIN_ECHO      = 5;   
const int PIN_SERVO     = 9;   

Servo radarServo;              

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // SELBSTTEST-LOGIK: Beide LEDs leuchten beim Start
  Serial.println("S:TESTING");
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  
  // Zentrierung
  radarServo.attach(PIN_SERVO);
  radarServo.write(90); 
  delay(1500); 
  
  // Ultraschall-Testmessung
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duration = pulseIn(PIN_ECHO, HIGH, 30000); 
  float distance = duration * 0.0343 / 2.0;
  
  if (distance > 0 && distance < 400) {
    // ERFOLG: Nur noch Grün leuchtet, Rot geht aus
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
    Serial.println("S:OK");
  } else {
    // FEHLER: Nur noch Rot leuchtet, Grün geht aus
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    Serial.println("S:ERR_SENSOR");
    while(1); 
  }
}

void loop() {
  for (int angle = 90; angle >= 30; angle--) { scanPosition(angle); }
  for (int angle = 30; angle <= 150; angle++) { scanPosition(angle); }
  for (int angle = 150; angle >= 90; angle--) { scanPosition(angle); }
}

void scanPosition(int angle) {
  radarServo.write(angle);
  delay(35); 
  
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  int distance = duration * 0.0343 / 2.0;
  
  Serial.print("D:");
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);
}