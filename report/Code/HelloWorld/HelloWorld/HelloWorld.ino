/**
 * @file HelloWorld.ino
 * @brief Minimalbeispiel zur Initialisierung der seriellen Schnittstelle
 *        und zur Ausgabe einer Testnachricht.
 * @details Das Programm dient zur Veranschaulichung der Verwendung von
 *          Serial.begin() und Serial.println() auf dem Arduino Nano 33 BLE Sense.
 *          Nach dem Start des Mikrocontrollers wird die serielle Schnittstelle
 *          initialisiert und einmalig die Zeichenkette "Hello World" im
 *          seriellen Monitor ausgegeben.
 * @author Simon Müller
 * @date 2026-06-23
 */

/**
 * @brief Initialisiert die serielle Schnittstelle und sendet eine Testausgabe.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; ///< Wartet, bis die serielle Schnittstelle bereit ist.
  }
  Serial.println("Hello World");
}

/**
 * @brief Enthält den zyklisch ausgeführten Programmteil.
 * @details In diesem Beispiel bleibt die Hauptschleife leer, da keine
 *          wiederkehrende Verarbeitung erforderlich ist.
 */
void loop() {
  // keine wiederkehrende Verarbeitung
}
\end{lstlisting}