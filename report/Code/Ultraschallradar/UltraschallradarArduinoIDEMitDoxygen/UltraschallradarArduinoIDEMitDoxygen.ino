/**
 * @file UltraschallradarArduinoIDEMitDoxygen.ino
 * @brief Firmware für ein automatisches 120-Grad-Ultraschallradar.
 * @details Diese Software steuert ein Ultraschallradar, bestehend aus einem 
 * Arduino Nano V3, einem HC-SR04 Ultraschallsensor und einem SG90 Positions-Servomotor.
 * Beim Systemstart wird eine Zentrierung auf 90° durchgeführt und ein visueller 
 * Power-On Self-Test (POST) über zwei Status-LEDs signalisiert. Im laufenden Betrieb
 * wird ein kontinuierlicher Schwenk von 30° bis 150° ausgeführt, die gemessene 
 * Distanz ermittelt und über das serielle Protokoll an das Processing-Frontend gesendet.
 * * @author Wilko Hinrichs
 * @date 2026-06-08
 * @version 1.1
 */

#include <Servo.h>

/* --- PIN-DEFINITIONEN --- */
const int PIN_LED_GREEN = 2;   ///< GPIO-Pin für die grüne Status-LED (System betriebsbereit)
const int PIN_LED_RED   = 3;   ///< GPIO-Pin für die rote Status-LED (Fehler / Selbsttest)
const int PIN_TRIG      = 4;   ///< Digitaler Ausgangs-Pin zur Initiierung des Ultraschall-Impulses (Trig)
const int PIN_ECHO      = 5;   ///< Digitaler Eingangs-Pin zur Messung der Schall-Laufzeit (Echo)
const int PIN_SERVO     = 9;   ///< PWM-Ausgangs-Pin zur Winkelsteuerung des SG90 Servomotors

/* --- GLOBALE OBJEKTE --- */
Servo radarServo;              ///< Servo-Instanz der offiziellen Arduino-Bibliothek zur Achsensteuerung.

/**
 * @brief Initialisiert die Hardware-Peripherie und führt den Boot-Selbsttest aus.
 * @details Konfiguriert die seriellen Kommunikationsparameter auf 9600 Baud, definiert 
 * die Pin-Modi (INPUT/OUTPUT) und leitet den Selbsttest ein, bei dem zunächst **beide** * LEDs leuchten. Der Servo wird auf die mathematische Mitte (90°) gefahren. Wird eine 
 * erfolgreiche Testmessung im gültigen physikalischen Bereich (0 - 400 cm) registriert, 
 * schaltet das System die rote LED aus und startet den Normalbetrieb. Andernfalls blockiert 
 * das System im Fehlermodus.
 */
void setup() {
  // Serielle Schnittstelle mit stabiler Standard-Baudrate initialisieren
  Serial.begin(9600);
  
  // Definition der Signalrichtungen
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // SELBSTTEST-BEGINN: Beide LEDs signalisieren den Testzustand
  Serial.println("S:TESTING");
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  
  // ZENTRIERUNG: Servo fährt in die exakte Mitte (90 Grad) für die Gehäusemontage
  radarServo.attach(PIN_SERVO);
  radarServo.write(90); 
  delay(1500); // Ausreichend Zeit, um den Sensorkopf mechanisch auszurichten
  
  // Ultraschall-Testmessung triggern (10 Mikrosekunden High-Impuls)
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Messung der Echo-Rücklaufzeit mit einem Timeout von 30ms (ca. 5 Meter Reichweite)
  long duration = pulseIn(PIN_ECHO, HIGH, 30000); 
  float distance = duration * 0.0343 / 2.0;
  
  // Validierung des Sensorsignals
  if (distance > 0 && distance < 400) {
    // SELBSTTEST ERFOLGREICH: Rot aus, Grün bleibt an -> Normalbetrieb
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
    Serial.println("S:OK");
  } else {
    // SELBSTTEST FEHLGESCHLAGEN: Grün aus, Rot bleibt an -> System sperren
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    Serial.println("S:ERR_SENSOR");
    while(1); // Endlosschleife verhindert die Ausführung der loop() bei Defekt
  }
}

/**
 * @brief Kontinuierliche Hauptschleife für die periodische Schwenkbewegung.
 * @details Realisiert die Schwenkbewegung des Radars symmetrisch um die Mittelachse (90°).
 * Der gesamte Scanbereich beträgt exakt 120 Grad und wird in drei Phasen durchlaufen:
 * 1. Schwenk von der Mitte (90°) nach ganz links zur Grenze (30°).
 * 2. Kompletter Schwenk von ganz links (30°) nach ganz rechts zur Grenze (150°).
 * 3. Rückschwenk von ganz rechts (150°) zurück in die Ausgangsmitte (90°).
 */
void loop() {
  // Phase 1: Von der Mitte nach links (90° bis 30°)
  for (int angle = 90; angle >= 30; angle--) { 
    scanPosition(angle); 
  }
  
  // Phase 2: Von links komplett nach rechts (30° bis 150°)
  for (int angle = 30; angle <= 150; angle++) { 
    scanPosition(angle); 
  }
  
  // Phase 3: Von rechts wieder zurück in die Mitte (150° bis 90°)
  for (int angle = 150; angle >= 90; angle--) { 
    scanPosition(angle); 
  }
}

/**
 * @brief Fährt einen spezifischen Winkel an, misst die Distanz und überträgt das Datenprotokoll.
 * @details Diese Funktion kapselt den gesamten Messvorgang für ein Gradsegment:
 * 1. Der Servo erhält den neuen Stellbefehl. Ein Delay von 35ms sichert der Mechanik 
 * des SG90 ausreichend Zeit zu, die Position vibrationsfrei zu erreichen.
 * 2. Der HC-SR04 sendet einen Ultraschall-Burst aus.
 * 3. Über die Schallgeschwindigkeit in Luft (\f$343\text{ m/s}\f$) wird aus der Laufzeit 
 * die Distanz in cm berechnet (\f$\text{Weg} = \frac{\text{Zeit} \times 0.0343}{2}\f$).
 * 4. Die Daten werden im standardisierten Stringformat `D:[Winkel],[Distanz]` über die 
 * serielle Schnittstelle (UART) an Processing übertragen.
 * * @param angle Der präzise anzufahrende Zielwinkel in Grad (30° bis 150°).
 */
void scanPosition(int angle) {
  radarServo.write(angle);
  delay(35); // Einschwingzeit für die Servomechanik (wichtig gegen Bildzittern)
  
  // Ultraschall-Messzyklus starten
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Pulsdauer am Echo-Pin messen
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  int distance = duration * 0.0343 / 2.0;
  
  // Protokollausgabe an das Processing-Frontend (z.B. "D:45,18")
  Serial.print("D:");
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);
}