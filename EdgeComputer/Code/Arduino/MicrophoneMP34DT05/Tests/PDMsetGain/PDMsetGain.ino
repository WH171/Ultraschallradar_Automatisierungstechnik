/**
 * @file Test_PDM_setGain.ino
 * @brief Test microphone gain adjustment
 * @note Physical verification required for actual level changes
 */

#include <PDM.h>

/**
 * @brief Configure gain before initialization
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  PDM.setGain(255);
  Serial.println("Gain set to 255");

  if (!PDM.begin(1, 16000)) {
    Serial.println("Init failed");
    while(1);
  }
  
  Serial.println("Test microphone sensitivity");
}

/**
 * @brief Empty loop
 */
void loop() {}