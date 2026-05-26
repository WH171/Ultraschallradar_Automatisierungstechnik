#include <Servo.h>

/**
 * @file RadarControl.ino
 * @brief Steuerungscode fuer das Ultraschallradar mit Arduino Nano 33 BLE.
 * * Dieser Code steuert einen SG90 Servomotor und liest Distanzen ueber einen
 * HC-SR04 Ultraschallsensor aus. Die Daten werden ueber die serielle
 * Schnittstelle an ein Processing-Frontend gesendet. Zudem geben zwei LEDs
 * (Rot/Gruen) visuelles Feedback ueber den Systemstatus.
 */

// --- PIN DEFINITIONEN ---
/** @brief Pin fuer den Trigger-Anschluss des Ultraschallsensors. */
const int trigPin = 4;
/** @brief Pin fuer den Echo-Anschluss des Ultraschallsensors (Spannungsteiler beachten!). */
const int echoPin = 5;
/** @brief PWM-Pin zur Steuerung des Servomotors. */
const int servoPin = 9;
/** @brief Pin fuer die gruene Status-LED (System OK). */
const int ledGreen = 2;
/** @brief Pin fuer die rote Status-LED (Systemfehler). */
const int ledRed = 3;

/** @brief Servo-Objekt zur Steuerung des Motors. */
Servo radarServo;

// --- SYSTEM VARIABLEN ---
/** @brief Gibt an, ob der Hardware-Selbsttest erfolgreich war. */
bool systemOk = false;
/** @brief Speichert den aktuellen Winkel des Servomotors (0 bis 180 Grad). */
int currentAngle = 0;
/** @brief Bestimmt die Drehrichtung des Servos (1 fuer Vorwaerts, -1 fuer Rueckwaerts). */
int direction = 1; 
/** @brief Maximale Messdistanz in cm (wichtig fuer die Skalierung im Radar). */
const int maxDistance = 40; 

/**
 * @brief Initialisiert die Pins, startet die serielle Kommunikation und fuehrt einen Selbsttest durch.
 * * Waehrend des Selbsttests (POST - Power-On Self-Test) werden die LEDs kurz aufgeleuchtet, 
 * der Servo macht eine Testdrehung und der Sensor macht eine Probemessung. 
 * Das Ergebnis ("S:OK" oder "S:ERR_SENSOR") wird an Processing gesendet.
 */
void setup() {
  // Pins konfigurieren
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  radarServo.attach(servoPin);
  radarServo.write(0); // Servo auf Startposition
  
  Serial.begin(9600);
  
  // WICHTIG FUER NANO 33 BLE: Warten, bis PC zuhoert
  while (!Serial);
  
  // --- START DES SELBSTTESTS (POST) ---
  Serial.println("S:TESTING");
  
  // 1. LED Test
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, HIGH);
  delay(1000);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  
  // 2. Servo Test (Einmal hin und her)
  for(int i = 0; i <= 180; i+=30) {
    radarServo.write(i);
    delay(100);
  }
  radarServo.write(0);
  delay(500);
  
  // 3. Sensor Test
  long testDist = getDistance();
  
  // Wenn Distanz 0 ist (Timeout) oder voellig unlogisch, Fehler melden
  if (testDist <= 0 || testDist > 400) { 
    systemOk = false;
    Serial.println("S:ERR_SENSOR");
    digitalWrite(ledRed, HIGH); // Rote LED dauerhaft AN
  } else {
    systemOk = true;
    Serial.println("S:OK");
    digitalWrite(ledGreen, HIGH); // Gruene LED dauerhaft AN
  }
}

/**
 * @brief Hauptschleife. Fuehrt den Radar-Scan durch, falls das System fehlerfrei ist.
 * * Bewegt den Servo schrittweise, misst die Distanz und sendet die Daten im Format
 * "D:Winkel,Distanz" an die serielle Schnittstelle. Prallt der Servo an den 
 * Grenzen (0 oder 180 Grad) ab, wird die Richtung umgekehrt.
 */
void loop() {
  if (systemOk) {
    // Radar bewegen
    radarServo.write(currentAngle);
    delay(30); // Kurze Pause, damit der Servo die Position erreicht
    
    // Distanz messen
    int distance = getDistance();
    
    // Daten an Processing senden (Format: D:Winkel,Distanz)
    Serial.print("D:");
    Serial.print(currentAngle);
    Serial.print(",");
    Serial.println(distance);
    
    // Winkel fuer den naechsten Durchlauf anpassen
    currentAngle += direction;
    if (currentAngle >= 180) {
      direction = -1;
    } else if (currentAngle <= 0) {
      direction = 1;
    }
  } else {
    // Im Fehlerfall passiert hier nichts weiter, ausser dass die rote LED leuchtet.
    // Das System muss neugestartet werden (Reset-Button).
    delay(1000);
  }
}

/**
 * @brief Misst die Distanz mithilfe des Ultraschallsensors.
 * * Sendet einen 10 Mikrosekunden langen Trigger-Impuls und misst die Zeit 
 * bis zum Eintreffen des Echos ueber pulseIn(). 
 * * @return Gemessene Distanz in Zentimetern (cm) oder 0 bei einem Timeout.
 */
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Timeout von 30.000 Mikrosekunden (ca. 5 Meter), verhindert Aufhaengen
  long duration = pulseIn(echoPin, HIGH, 30000); 
  
  if (duration == 0) return 0; // Timeout = Fehler
  
  long dist = duration * 0.034 / 2; // Umrechnung in cm
  return dist;
}