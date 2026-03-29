/**
 * @file Test_PDM_available.ino
 * @brief Test audio buffer monitoring
 */

#include <PDM.h>

/**
 * @brief Initialize PDM and serial monitor
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!PDM.begin(1, 16000)) {
    Serial.println("Init failed");
    while(1);
  }
}

/**
 * @brief Continuously display available bytes
 */
void loop() {
  Serial.print("Available: ");
  Serial.println(PDM.available());
  delay(1000);
}