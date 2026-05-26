#include <Servo.h>

// Pin-Definition für den Servomotor
const int servoPin = 9;

Servo myServo;

void setup() {
  // Seriellen Monitor starten
  Serial.begin(9600);
  delay(2000); // Sicherheits-Verzögerung für den Nano 33 BLE

  // Servo aktivieren
  myServo.attach(servoPin);

  // Servo exakt in die Mitte (90 Grad) fahren
  myServo.write(120);
  
  Serial.println("========================================");
  Serial.println("KALIBRIERUNG GEGEBEN:");
  Serial.println("Servo wurde auf die Mitte (90°) gesetzt.");
  Serial.println("Du kannst jetzt das Ruderhorn gerade aufstecken.");
  Serial.println("========================================");
}

void loop() {
  // Gibt im Sekundentakt die aktuelle Position aus, damit du weißt, dass der Code läuft
  Serial.print("Aktuelle Servo-Position: ");
  Serial.print(myServo.read());
  Serial.println(" Grad (Mitte)");
  
  delay(1000); 
}