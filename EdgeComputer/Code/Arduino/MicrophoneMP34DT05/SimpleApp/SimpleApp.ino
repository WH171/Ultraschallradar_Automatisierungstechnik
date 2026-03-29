/** 
 * @file ClapDetector.ino
 * @brief Clap detection system using PDM microphone and LED feedback
 */

#include <PDM.h>

/** @brief Buffer to store microphone data samples */
short sampleBuffer[256];

/** @brief Number of samples read from the microphone (volatile for interrupt safety) */
volatile int samplesRead = 0;

/** @brief Pin number for the built-in LED */
const int ledPin = LED_BUILTIN;

/** @brief Flag indicating if LED is currently active */
bool ledActive = false;

/** @brief Timestamp when LED was last activated */
unsigned long ledStartTime = 0;

/** @brief Duration in milliseconds to keep LED on after detection */
const int ledDuration = 5000;

/** @brief Sound threshold for clap detection (auto-calibrated) */
int threshold = 0;

/** @brief Size of the peak history buffer for noise filtering */
const int historySize = 3;

/** @brief Circular buffer storing recent audio peaks */
int peakHistory[historySize];

/** @brief Current position in the circular buffer */
int historyIndex = 0;

/** @brief Last trigger timestamp for cooldown handling */
unsigned long lastTriggerTime = 0;

/** @brief Minimum delay between clap detections in milliseconds */
const int cooldown = 300;

/** 
 * @brief Initializes serial communication, microphone, and performs threshold calibration. 
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT);
  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, 16000)) {
    Serial.println("PDM init failed!");
    while (1);
  }

  calibrateThreshold();
  Serial.print("Auto threshold: ");
  Serial.println(threshold);
  Serial.println("Waiting for clap...");
}

/** 
 * @brief Main loop for detecting claps and controlling the LED. 
 */
void loop() {
  if (samplesRead > 0) {
    int maxAmplitude = getMaxAmplitude();
    updatePeakHistory(maxAmplitude);
    samplesRead = 0;

    int recentMax = getRecentMax();
    if (recentMax > threshold && (millis() - lastTriggerTime > cooldown)) {
      triggerLED();
      lastTriggerTime = millis();
      Serial.print("Clap detected! Strength: ");
      Serial.println(recentMax);
    }
  }

  handleLEDTimeout();
}

/** 
 * @brief Calibrates the clap detection threshold by measuring ambient sound. 
 */
void calibrateThreshold() {
  Serial.println("Calibrating... (2 seconds of silence)");
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

/**
 * @brief Calculates the maximum amplitude in the current sample buffer.
 * @return Maximum absolute value from the buffer.
 */
int getMaxAmplitude() {
  int maxVal = 0;
  for (int i = 0; i < samplesRead; i++) {
    int val = abs(sampleBuffer[i]);
    if (val > maxVal) maxVal = val;
  }
  return maxVal;
}

/**
 * @brief Updates the history buffer with a new peak value.
 * @param peak The new peak value to add.
 */
void updatePeakHistory(int peak) {
  peakHistory[historyIndex] = peak;
  historyIndex = (historyIndex + 1) % historySize;
}

/**
 * @brief Returns the highest peak from the recent history.
 * @return Maximum peak from the history buffer.
 */
int getRecentMax() {
  int recentMax = 0;
  for (int i = 0; i < historySize; i++) {
    if (peakHistory[i] > recentMax) recentMax = peakHistory[i];
  }
  return recentMax;
}

/**
 * @brief Turns on the LED and records the time for timeout control.
 */
void triggerLED() {
  digitalWrite(ledPin, HIGH);
  ledActive = true;
  ledStartTime = millis();
}

/**
 * @brief Turns off the LED if the timeout duration has passed.
 */
void handleLEDTimeout() {
  if (ledActive && (millis() - ledStartTime >= ledDuration)) {
    digitalWrite(ledPin, LOW);
    ledActive = false;
    Serial.println("LED off");
  }
}

/**
 * @brief Callback function triggered when new PDM data is available.
 */
void onPDMdata() {
  samplesRead = PDM.read(sampleBuffer, 256);
}
