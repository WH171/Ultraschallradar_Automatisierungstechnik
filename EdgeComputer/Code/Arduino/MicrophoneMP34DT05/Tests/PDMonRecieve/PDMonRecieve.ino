/**
 * @file Test_PDM_onReceive.ino
 * @brief Test interrupt-driven audio capture
 */

#include <PDM.h>

/** @brief Data reception flag */
volatile bool dataReceived = false;

/**
 * @brief Callback trigger function
 * @warning Keep callbacks short for stable operation
 */
void onDataAvailable() {
  dataReceived = true;
}

/**
 * @brief Register callback and initialize PDM
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  PDM.onReceive(onDataAvailable);
  
  if (!PDM.begin(1, 16000)) {
    Serial.println("Init failed");
    while(1);
  }
}

/**
 * @brief Monitor callback events
 */
void loop() {
  if (dataReceived) {
    dataReceived = false;
    Serial.println("Data received");
  }
}