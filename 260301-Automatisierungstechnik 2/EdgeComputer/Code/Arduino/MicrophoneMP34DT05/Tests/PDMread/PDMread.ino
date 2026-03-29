/**
 * @file Test_PDM_read.ino
 * @brief Test audio data retrieval
 * @note Shows first 5 bytes in hex format
 */

#include <PDM.h>

/** @brief Buffer size for audio data */
#define BUFFER_SIZE 512
/** @brief Audio data storage */
uint8_t buffer[BUFFER_SIZE];

/**
 * @brief PDM initialization
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
 * @brief Read and display audio data
 */
void loop() {
  int bytes = PDM.available();
  if (bytes > 0) {
    int read = PDM.read(buffer, BUFFER_SIZE);
    Serial.print("Read: ");
    Serial.println(read);
    
    for (int i = 0; i < min(5, read); i++) {
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}