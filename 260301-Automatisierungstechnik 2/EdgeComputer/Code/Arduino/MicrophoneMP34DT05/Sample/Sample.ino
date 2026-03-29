#include <PDM.h>

/**
 * @def SAMPLE_RATE
 * @brief Audio sample rate in Hz.
 */
#define SAMPLE_RATE     16000

/**
 * @def CHANNELS
 * @brief Number of audio channels (1 = mono).
 */
#define CHANNELS        1

/**
 * @def BUFFER_SIZE
 * @brief Size of the sample buffer (in 16-bit samples).
 */
#define BUFFER_SIZE     256

/**
 * @brief Global buffer to hold audio samples from the microphone.
 */
short sampleBuffer[BUFFER_SIZE];

/**
 * @brief Prints an error message to the serial console and halts execution.
 * 
 * This function is typically used when microphone initialization fails.
 * 
 * @param message The error message to display.
 */
void printError(const char* message) {
  Serial.print("ERROR: ");
  Serial.println(message);
  while (1) { delay(100); }
}

/**
 * @brief Initializes the serial interface and the PDM microphone.
 * 
 * Sets up communication via USB serial, configures the PDM microphone
 * with the defined sample rate and number of channels, and sets an initial gain.
 * If initialization fails, the system halts with an error message.
 */
void setup() {
  Serial.begin(115200);
  while (!Serial);

  PDMConfig config = {
    .channels = CHANNELS,
    .frequency = SAMPLE_RATE,
  };

  if (!PDM.begin(config)) {
    printError("PDM initialization failed!");
  }

  PDM.setGain(40);

  Serial.println("PDM microphone ready");
  Serial.println("Sampling rate\tChannels\tBuffer");
  Serial.print(SAMPLE_RATE); Serial.print("\t\t");
  Serial.print(CHANNELS); Serial.print("\t\t");
  Serial.println(BUFFER_SIZE);
}

/**
 * @brief Continuously reads audio data from the PDM microphone and prints sample values.
 * 
 * Reads available audio samples into a buffer, then prints the sample values
 * to the serial monitor at 100ms intervals. Limits printing frequency to avoid flooding.
 */
void loop() {
  static unsigned long lastPrint = 0;
  int bytesAvailable = PDM.available();

  if (bytesAvailable > 0) {
    int samplesAvailable = bytesAvailable / 2;
    int readSamples = PDM.read(sampleBuffer, min(samplesAvailable, BUFFER_SIZE));

    if (millis() - lastPrint >= 100) {
      Serial.print("Samples: ");
      for (int i = 0; i < readSamples; i++) {
        Serial.println(sampleBuffer[i]);
      }
      lastPrint = millis();
    }
  }
}
