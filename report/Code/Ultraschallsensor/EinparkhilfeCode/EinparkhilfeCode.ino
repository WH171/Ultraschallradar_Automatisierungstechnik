/**
 * @file Einparkhilfe.ino
 * @brief Automatisierte Einparkhilfe mit Ultraschallsensor HC-SR04.
 * @details Nutzt die NewPing-Bibliothek von Tim Eckel zur Distanzmessung.
 * Das System warnt optisch per LED und akustisch per Buzzer.
 * @author Dein Name
 * @date 2026-04-28
 */

#include <NewPing.h>

/** @name Pin-Konfiguration 
 * Zuweisung der digitalen Pins am Arduino
 * @{ */
#define TRIGGER_PIN  12  ///< Trigger-Pin des HC-SR04
#define ECHO_PIN     11  ///< Echo-Pin des HC-SR04
#define LED_PIN      13  ///< Pin für die Warn-LED
#define BUZZER_PIN   10  ///< Pin für den akustischen Buzzer
/** @} */

/** @name Schwellenwerte
 * Distanzgrenzen für die Warnlogik in Zentimetern
 * @{ */
#define MAX_DISTANCE 200 ///< Maximale Sensorreichweite
#define DISTANCE_WARN 50 ///< Schwellenwert für optische Warnung (LED)
#define DISTANCE_STOP 10 ///< Schwellenwert für akustischen Alarm (Buzzer)
/** @} */

/** * @brief Initialisierung des NewPing-Objekts.
 * @param TRIGGER_PIN Pin für das Sendesignal.
 * @param ECHO_PIN Pin für das Empfangssignal.
 * @param MAX_DISTANCE Begrenzung der Messung zur Zeitersparnis.
 */
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

/**
 * @brief Konfiguriert die Hardware-Pins und startet die serielle Kommunikation.
 */
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

/**
 * @brief Hauptschleife: Führt zyklische Messungen durch und steuert die Aktoren.
 * @details Die Messung erfolgt alle 50ms (20 Hz). 
 * Eine Distanz von 0 cm wird als "außerhalb der Reichweite" interpretiert.
 */
void loop() {
  // Kurze Pause zwischen den Messungen für stabilere Werte
  delay(50);

  /** * @var unsigned int distance 
   * @brief Aktuell gemessene Distanz zum Objekt in cm.
   */
  unsigned int distance = sonar.ping_cm();

  // Ausgabe der Messwerte zur Diagnose
  Serial.print("Messwert: ");
  Serial.print(distance);
  Serial.println(" cm");

  checkDistanceLogic(distance);
}

/**
 * @brief Verarbeitet die gemessene Distanz und schaltet Warnsignale.
 * @param dist Die aktuelle Distanz in Zentimetern.
 * @note Werte von 0 werden ignoriert, da sie Fehlmessungen oder "Out of Range" darstellen.
 */
void checkDistanceLogic(unsigned int dist) {
  if (dist > 0 && dist < DISTANCE_STOP) {
    // Kritischer Bereich: Dauerwarnung
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } 
  else if (dist > 0 && dist < DISTANCE_WARN) {
    // Warnbereich: Nur optisches Signal
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  else {
    // Sicherer Bereich oder keine Messung
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}