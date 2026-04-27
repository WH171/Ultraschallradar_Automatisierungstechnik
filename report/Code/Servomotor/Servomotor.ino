/**
 * @file RadarServo.ino
 * @brief Servo-basiertes Radar-Schwenksystem
 * 
 * @details Dieses Programm steuert einen Servomotor, der sich kontinuierlich
 * zwischen 0 und 180 Grad hin- und herbewegt. Typische Anwendung für 
 * Radar-Scanning-Systeme mit Ultraschallsensoren.
 * 
 * @author Simon Müller
 * @date 27.04.2026
 * @version 1.0
 */

#include <Servo.h>

Servo radarServo;  ///< Servo-Objekt für die Radarsteuerung
int pos = 0;       ///< Aktuelle Servo-Position (0-180 Grad)

/**
 * @brief Initialisierung beim Programmstart
 * 
 * @details Verbindet den Servomotor mit Pin 9 und bereitet
 * ihn für die Steuerung vor.
 * 
 * @return void
 */
void setup() {
    radarServo.attach(9);  // Servo an Pin 9 anschließen
}

/**
 * @brief Hauptschleife für kontinuierliche Servo-Bewegung
 * 
 * @details Schwenkt den Servo in zwei Phasen:
 * - Phase 1: Bewegung von 0° nach 180° in 1°-Schritten
 * - Phase 2: Bewegung von 180° zurück nach 0° in 1°-Schritten
 * 
 * Gesamtdauer pro Zyklus: ca. 5,4 Sekunden
 * 
 * @return void
 */
void loop() {
    // Schwenkt von 0 Grad auf 180 Grad
    for (pos = 0; pos <= 180; pos += 1) { 
        radarServo.write(pos);  ///< Setzt Servo auf Position 'pos'
        delay(15);              ///< Wartet 15ms für das Erreichen der Position
    }
    
    // Schwenkt von 180 Grad zurück auf 0 Grad
    for (pos = 180; pos >= 0; pos -= 1) { 
        radarServo.write(pos);  ///< Setzt Servo auf Position 'pos'
        delay(15);              ///< Wartet 15ms
    }
}