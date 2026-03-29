/**
 * @file Test_PDM_end.ino
 * @brief Example demonstrating the use of the PDM module.
 */

#include <PDM.h>

/**
 * @brief Initializes serial communication and controls the PDM module.
 *
 * This function sets up serial communication at 9600 baud, waits for the serial monitor,
 * starts the PDM module in mono mode with a 16 kHz sample rate, prints a message to the console,
 * ends the PDM session, prints another message, and then attempts to reinitialize the module,
 * confirming the success via serial output.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  /** Initialize the PDM module in mono mode with 16 kHz sample rate */
  PDM.begin(1, 16000);
  Serial.println("PDM started");

  /** End the current PDM session */
  PDM.end();
  Serial.println("PDM stopped");

  /** Attempt to reinitialize the PDM module */
  if (PDM.begin(1, 16000)) {
    Serial.println("PDM.begin() successful on reinitialization");
  }
}
