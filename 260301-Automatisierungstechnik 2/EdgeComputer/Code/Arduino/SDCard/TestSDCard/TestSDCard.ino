#include <SPI.h>
#include <SD.h>

/// Chip select pin for the SD card module
const int chipSelect = 8;

/**
 * @brief Initializes serial communication and SD card,
 *        then writes example data to a CSV file.
 */
void setup() {
  Serial.begin(9600);
  while(!Serial){
    Serial.println("Hello");
  // Initialize SD card
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card detected.");

  // Open or create the CSV file
  File file = SD.open("daten.csv", FILE_WRITE);

  // If file is available, write header and example values
  if (file) {
    file.println("Time,Value");
    file.println("00:00:01,200");
    file.println("00:00:02,200");
    file.close();
    Serial.println("Data written to CSV.");
  } else {
    Serial.println("Error opening file!");
  }
}
}
/**
 * @brief Main loop function.
 *        Not used in this example.
 */
void loop() {
  // Do nothing
    
}
