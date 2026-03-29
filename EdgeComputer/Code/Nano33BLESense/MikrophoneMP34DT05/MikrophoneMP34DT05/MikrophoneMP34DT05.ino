/**
* @file PDM_Microphone.ino
* @brief Example to capture and process audio data using PDM microphone.
*
* This example demonstrates how to initialize the PDM microphone, 
* read audio samples into a buffer, and print them to the Serial Monitor.
* 
* @note Ensure the PDM library is installed, and the board supports PDM input.
*
* @author Public Domain
*/
	
#include <PDM.h> ///< Include the library for PDM microphone.
	
void setup() {
	Serial.begin(9600); ///< Start Serial communication at 9600 baud rate.
		
	// Initialize the PDM microphone with 1 channel and 16 kHz sample rate.
	if (!PDM.begin(1, 16000)) {
		Serial.println("Failed to start PDM!"); ///< Error message if initialization fails.
		while (1); ///< Halt the program if PDM initialization fails.
	}
}
	
void loop() {
	// Check how many bytes are available in the buffer.
	int bytesAvailable = PDM.available();
	if (bytesAvailable > 0) {
		short buffer[bytesAvailable]; ///< Declare a buffer to hold audio samples.
			
		// Read the audio samples into the buffer.
		PDM.read(buffer, bytesAvailable);
			
		// Print each sample to the Serial Monitor.
		for (int i = 0; i < bytesAvailable / 2; i++) {
			Serial.println(buffer[i]);
		}
	}
}
