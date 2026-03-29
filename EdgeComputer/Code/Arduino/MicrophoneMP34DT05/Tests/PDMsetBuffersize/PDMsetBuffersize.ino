/**
 * @file Test_PDM_setBufferSize.ino
 * @brief Test buffer configuration
 * @warning Must be called before PDM.begin()
 */

#include <PDM.h>

/**
 * @brief Configure buffer size first
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  PDM.setBufferSize(1024);
  Serial.println("Buffer size set to 1024");

  if (!PDM.begin(1, 16000)) {
    Serial.println("Init failed");
    while(1);
  }
  
  Serial.println("Buffer configured");
}

/**
 * @brief Empty loop
 */
void loop() {}