/**
 * @file Test_PDM_begin.ino
 * @brief Example code for initializing the PDM module and serial communication.
 */

#include <PDM.h>

/**
 * @brief Initializes the serial communication and the PDM module.
 * 
 * This function sets up serial communication at 9600 baud and waits for the serial monitor to connect.
 * Then attempts to initialize the PDM module in mono mode at 16 kHz sampling rate.
 * 
 * @details The initialization process includes:
 * - Starting serial communication
 * - Waiting for serial connection
 * - Configuring PDM for single-channel audio input
 * - Providing success/failure feedback via serial interface
 */
void setup() {
  Serial.begin(9600);
  
  /** Wait for the serial monitor to connect */
  while (!Serial);
  
  /** Attempt to initialize PDM in mono mode at 16 kHz */
  if (PDM.begin(1, 16000)) {
    Serial.println("PDM.begin() successful (Mono, 16 kHz)");
  } else {
    Serial.println("PDM.begin() failed!");
  }
}
