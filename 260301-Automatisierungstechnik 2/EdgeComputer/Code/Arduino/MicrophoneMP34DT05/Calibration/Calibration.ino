#include <PDM.h>

/**
 * @brief Buffer to hold audio samples from the PDM microphone.
 */
short sampleBuffer[256];

/**
 * @brief Number of samples read from the microphone.
 * 
 * This value is updated inside the onPDMdata() callback
 * whenever new audio data is available.
 */
volatile int samplesRead = 0;

/**
 * @brief Threshold value calculated during calibration.
 * 
 * Used as a reference level for distinguishing meaningful
 * audio from background noise.
 */
int threshold = 0;

/**
 * @brief Callback function called automatically when new PDM data is available.
 * 
 * This function reads audio samples from the microphone
 * into the sample buffer and updates the samplesRead counter.
 */
void onPDMdata() {
  samplesRead = PDM.read(sampleBuffer, 256);
}

/**
 * @brief Sets up serial communication, initializes the microphone,
 *        and performs initial calibration to determine the audio threshold.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing microphone...");

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, 16000)) {
    Serial.println("PDM init failed!");
    while (1);
  }

  calibrateThreshold();

  Serial.print("Calibration complete! Threshold value: ");
  Serial.println(threshold);
}

/**
 * @brief Main loop function.
 * 
 * After calibration, no further actions are taken in the loop.
 */
void loop() {
}

/**
 * @brief Calibrates the microphone by measuring the ambient sound level.
 * 
 * Samples audio input over a 2-second period and calculates the average
 * signal magnitude. The resulting value is scaled to define a threshold
 * that can be used for voice activity detection or noise filtering.
 */
void calibrateThreshold() {
  Serial.println("Calibrating... Please stay quiet for 2 seconds.");
  delay(2000);

  long total = 0;
  int samplesCounted = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < 2000) {
    if (samplesRead > 0) {
      for (int i = 0; i < samplesRead; i++) {
        total += abs(sampleBuffer[i]);
        samplesCounted++;
      }
      samplesRead = 0;
    }
  }

  if (samplesCounted > 0) {
    threshold = (total / samplesCounted) * 3;
  } else {
    threshold = 15000;
  }
}
