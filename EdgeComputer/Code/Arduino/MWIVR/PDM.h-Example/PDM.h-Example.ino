#include <PDM.h>

/**
 * @file PDM.h-Example.ino
 * @brief Initializes and handles the PDM microphone on the Arduino Nano 33 BLE Sense.
 * 
 * This file contains the setup and data receive callback for processing audio data
 * using the built-in PDM microphone.
 */

/**
 * @brief Callback function that is automatically called when new PDM data is available.
 * 
 * This function is registered with the PDM interface and is triggered whenever
 * audio data is received from the microphone.
 */
void onPDMdata() {
    // Handle incoming audio data
}

/**
 * @brief Initializes the PDM microphone.
 * 
 * Configures the microphone to use 1 channel with a 16 kHz sampling rate.
 * Sets the gain and buffer size, and registers the receive callback.
 */
void setup() {
    PDM.onReceive(onPDMdata);      ///< Register callback for incoming data
    PDM.setBufferSize(512);        ///< Set buffer size to 512 bytes
    PDM.setGain(127);              ///< Set microphone gain to maximum
    PDM.begin(1, 16000);           ///< Start PDM with 1 channel at 16 kHz
}