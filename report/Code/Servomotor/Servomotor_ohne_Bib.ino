/**
 * @file ServoManual.ino
 * @brief Manuelle Servosteuerung ohne Bibliothek
 */

int servoPin = 9;  ///< Pin-Nummer für Servo-Ansteuerung

/**
 * @brief Initialisierung beim Programmstart
 * 
 * @details Konfiguriert den Servo-Pin als Ausgang (OUTPUT),
 * damit PWM-Signale gesendet werden können.
 * 
 * @return void
 */
void setup() {
    pinMode(servoPin, OUTPUT);  // Servo-Pin als Ausgang setzen
}

/**
 * @brief Hauptschleife für kontinuierliche PWM-Erzeugung
 * 
 * @details Erzeugt manuell ein 50-Hz-PWM-Signal (20 ms Periode).
 * Der HIGH-Puls von 1,5 ms entspricht einer Servo-Position von ca. 90°.
 * 
 * PWM-Timing:
 * - HIGH-Phase: 1500 µs (1,5 ms) → 90° Position
 * - LOW-Phase: 18500 µs (18,5 ms) → Auffüllen auf 20 ms Gesamtperiode
 * - Frequenz: 50 Hz (1000000 µs / 20000 µs = 50 Hz)
 * 
 * @note Diese Methode blockiert den Programmablauf. Für komplexere
 * Anwendungen sollte die Servo-Bibliothek verwendet werden.
 * 
 * @return void
 */
void loop() {
    // Bewegung auf ca. 90 Grad (1.5ms HIGH-Puls)
    digitalWrite(servoPin, HIGH);    ///< Startet den HIGH-Puls
    delayMicroseconds(1500);         ///< Pulsweite: 1,5 ms → 90° Position
    digitalWrite(servoPin, LOW);     ///< Beendet den HIGH-Puls
    
    // Auffüllen der restlichen 20ms Periode (50 Hz)
    delayMicroseconds(18500);        ///< LOW-Phase: 18,5 ms bis zur nächsten Periode
}