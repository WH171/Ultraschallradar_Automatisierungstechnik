/**
 * @file ArduinoSerialUebertragung.ino
 * @brief Captures audio data from a PDM microphone at 16 kHz sample rate.
 * 
 * This program continuously reads audio samples from a PDM microphone and 
 * transmits the raw data via the serial interface.
 */

#include <PDM.h>

#define SAMPLE_RATE   16000   /**< Sampling rate in Hz */
#define BUFFER_SIZE   256     /**< Size of the audio buffer in samples */

int16_t sampleBuffer[BUFFER_SIZE];       /**< Buffer to hold audio samples */
volatile int samplesRead = 0;            /**< Number of samples read from PDM */

/**
 * @brief Initializes serial communication and sets up the PDM microphone.
 */
void setup() {
  Serial.begin(250000);
  while (!Serial);

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, SAMPLE_RATE)) {
    Serial.println("Error: Microphone not detected!");
    while (1);
  }
}

/**
 * @brief Sends the captured audio data over the serial connection.
 */
void loop() {
  if (samplesRead > 0) {
    Serial.write((uint8_t*)sampleBuffer, samplesRead * sizeof(int16_t));
    samplesRead = 0;
  }
}

/**
 * @brief Callback function triggered when PDM data is available.
 * 
 * Reads the available bytes from the PDM interface and stores them in the buffer.
 */
void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
