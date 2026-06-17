/**
 * @file RadarControl.ino
 * @brief Synchronisierte Radarsteuerung mit erweitertem LED-Status vor dem Handshake.
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
  
  // ZUSTAND 1: Angesteckt, aber Processing ist noch ZU -> ROT leuchtet permanent
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
  
  // HANDSHAKE: Warte aktiv, bis das Processing-Programm geöffnet wird und "P:START" sendet
  while (true) {
    if (Serial.available() > 0) {
      String msg = Serial.readStringUntil('\n');
      if (msg.indexOf("P:START") != -1) {
        break; // Verbindung steht! Aus der Schleife ausbrechen.
      }
    }
    delay(100);
  }
  
  // ZUSTAND 2: SELBSTTEST STARTET (Processing ist offen) -> Beide LEDs gehen an
  Serial.println("S:TESTING");
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  
  radarServo.attach(PIN_SERVO);
  
  // SANFT ANFAHREN: Der Servo fährt langsam in die Mitte (90 Grad)
  int startWinkel = radarServo.read();
  if(startWinkel < 0 || startWinkel > 180) startWinkel = 30; 
  
  if (startWinkel < 90) {
    for (int w = startWinkel; w <= 90; w++) {
      radarServo.write(w);
      delay(25); 
    }
  } else {
    for (int w = startWinkel; w >= 90; w--) {
      radarServo.write(w);
      delay(25);
    }
  }
  delay(500); 
  
  // Ultraschall-Testmessung
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duration = pulseIn(PIN_ECHO, HIGH, 30000); 
  float distance = duration * 0.0343 / 2.0;
  
  if (distance > 0 && distance < 400) {
    // ZUSTAND 3: ERFOLG -> Normalbetrieb (Grün an, Rot aus)
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
    Serial.println("S:OK");
  } else {
    // REINER SENSOR-FEHLER -> Nur Rot leuchtet
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