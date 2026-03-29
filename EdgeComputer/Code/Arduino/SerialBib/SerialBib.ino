#include <SoftwareSerial.h>

/**
 * @brief SoftwareSerial-Objekt mit RX an Pin 10 und TX an Pin 11.
 * 
 * Diese Instanz stellt eine serielle Kommunikation zwischen
 * dem Arduino und einem externen seriellen Geraet (z.B. Bluetooth)
 * her.
 */
SoftwareSerial mySerial(10, 11);

/**
 * @brief Initialisiert das SoftwareSerial-Objekt und sendet eine
 *        Begruessungsnachricht.
 * 
 * Diese Funktion wird einmalig im Setup aufgerufen und startet die
 * serielle Kommunikation mit einer Baudrate von 9600. Es wird eine
 * Nachricht an das serielle Geraet gesendet, die anzeigt, dass die
 * Kommunikation aktiv ist.
 */
void setup() {
  // Starte SoftwareSerial mit 9600 Baud
  mySerial.begin(9600);

  // Initiale Nachricht senden
  mySerial.println("SoftwareSerial aktiv!");

  // Verbindungsstatus pruefen
  if (mySerial.isListening()) {
    mySerial.println("Verbindung aktiv.");
  }
  else {
    mySerial.println("Verbindung inaktiv.");
  }
}

/**
 * @brief Liest empfangene Daten, sendet diese zurueck und prueft
 *        auf Verfuegbarkeit.
 * 
 * In der Loop-Funktion wird kontinuierlich auf eingehende Daten
 * gewartet. Falls Daten empfangen werden, werden sie gelesen und
 * mit der Nachricht "Empfangen: " zurueckgesendet.
 * 
 * Anschliessend wird eine kurze Pause eingelegt, um die Belastung
 * des Mikrocontrollers zu reduzieren.
 */
void loop() {
  // Wenn Daten empfangen wurden
  if (mySerial.available() > 0) {
    // Ein Zeichen lesen
    char eingang = mySerial.read();

    // Zeichen zuruecksenden (Echo)
    mySerial.print("Empfangen: ");
    mySerial.println(eingang);
  }

  // Daten an Geraet senden
  mySerial.print("Nachricht: Hallo, wie geht's?");
  
  // Wenn keine Daten zum Empfangen vorhanden sind
  if (mySerial.available() == 0) {
    mySerial.println("Keine Daten empfangen.");
  }

  // Zuruecksetzen des Empfangspuffers
  mySerial.listen();

  // Kurze Pause zur Entlastung
  delay(100);
}
