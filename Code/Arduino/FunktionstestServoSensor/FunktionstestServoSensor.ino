#include <Servo.h>

// Pin-Definitionen
const int trigPin = 6;
const int echoPin = 4;
const int servoPin = 11;

Servo myServo;

void setup() {
  Serial.begin(9600);
  delay(2000); // Sicherheits-Verzögerung für Nano 33 BLE

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myServo.attach(servoPin);

  // --- KALIBRIERUNG & STARTPOSITION ---
  // Wir fahren die exakte Mitte an (Sensor schaut geradeaus)
  myServo.write(92); 
  
  Serial.println("Bringe Servo in Ausgangsposition (Geradeaus)...");
  delay(5000); // 5 Sekunden Zeit, um das Ruderhorn mechanisch gerade aufzustecken!
  Serial.println("Start des Radar-Scans!");
}

void loop() {
  // 1. Schwenk von der Mitte (90°) nach ganz links (0°)
  for (int angle = 90; angle >= 20; angle -= 2) {
    scanAtAngle(angle);
  }
  
  // 2. Schwenk von ganz links (0°) über die Mitte nach ganz rechts (180°)
  for (int angle = 20; angle <= 160; angle += 2) {
    scanAtAngle(angle);
  }

  // 3. Schwenk von ganz rechts (180°) zurück zur Mitte (90°)
  for (int angle = 160; angle >= 90; angle -= 2) {
    scanAtAngle(angle);
  }
}

// Hilfsfunktion: Fährt den Winkel an, wartet kurz und misst
void scanAtAngle(int angle) {
  myServo.write(angle);
  delay(40); // Zeit für die Bewegung
  
  int distance = getDistance();
  printData(angle, distance);
}

// Funktion zur präzisen Distanzmessung
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  int distance = duration * 0.0343 / 2;
  
  if (distance <= 0 || distance > 400) {
    return -1; 
  }
  return distance;
}

// Funktion für die Textausgabe
void printData(int angle, int distance) {
  Serial.print("Winkel: ");
  Serial.print(angle);
  Serial.print(" Grad | Abstand: ");
  
  if (distance == -1) {
    Serial.println("Außer Reichweite");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }
}