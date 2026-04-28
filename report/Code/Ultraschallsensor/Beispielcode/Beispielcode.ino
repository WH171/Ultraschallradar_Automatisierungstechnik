#include <NewPing.h>

/** @brief Digitaler Pin für das Trigger-Signal */
#define TRIGGER_PIN  12  

/** @brief Digitaler Pin für das Echo-Signal */
#define ECHO_PIN     11  

/** @brief Maximale Distanz, auf die der Sensor reagieren soll (in cm) */
#define MAX_DISTANCE 200 

/** * @brief Initialisierung des NewPing-Objekts.
 * @param TRIGGER_PIN Der konfigurierte Sende-Pin.
 * @param ECHO_PIN Der konfigurierte Empfangs-Pin.
 * @param MAX_DISTANCE Die definierte maximale Reichweite.
 */
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200); 
  Serial.println("System gestartet. Beginne Messung...");
}

void loop() {
  delay(50); // Pause zwischen Pings (Sicherer Abstand zum 20ms Intervall)

  /** * @brief Holt den Median aus 5 Messungen (in Mikrosekunden). */
  unsigned int uS = sonar.ping_median(5);

  Serial.print("Gefilterte Distanz: ");
  
  if (uS == 0) {
    Serial.println("Außerhalb der Reichweite");
  } else {
    // Umrechnung von Mikrosekunden in Zentimeter
    Serial.print(uS / US_ROUNDTRIP_CM); 
    Serial.println(" cm");
  }
}