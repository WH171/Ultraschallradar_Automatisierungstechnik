/**
 * @file RemoteXY_LED_Control.ino
 * @brief Remote-controlled LED using Bluetooth and RemoteXY interface
 * @details
 * Rundenzaehler fuer die Carrera-Bahn
 * This implementation demonstrates wireless LED control through a smartphone app
 * using the RemoteXY library. The system establishes Bluetooth communication
 * via SoftwareSerial (pins 10/RX, 11/TX) and provides a simple interface with
 * one button widget to toggle an LED connected to pin 13. The solution shows
 * basic RemoteXY integration patterns that can be expanded for more complex
 * IoT control applications. The LED state changes immediately when the virtual
 * button is pressed in the mobile app, with status feedback provided through
 * both the hardware LED and app interface.
 * 
 * @author Gruppe 2
 * @date 2025-06
 */

#include <Wire.h>
#include <RemoteXY.h>
#include <SoftwareSerial.h>

// Define the SoftwareSerial communication pins
SoftwareSerial mySerial(10, 11); // RX, TX pins for serial communication
RemoteXY_Init();

/**
 * @brief Pin for the onboard LED.
 * 
 * This is the pin where the LED is connected. 
 * It may vary depending on the Arduino model.
 */
#define LED_PIN 13 

// RemoteXY widget definition
/**
 * @brief RemoteXY button to control the LED.
 * 
 * A button widget that turns the LED on or off when pressed.
 */
RemoteXY_DECLARE_WIDGET(button1, REMOTEXY_BUTTON, 0, 0, 100, 40, "Turn LED On/Off");

/**
 * @brief Setup function to initialize hardware and communication.
 * 
 * This function sets up the LED pin, initializes serial communication,
 * and prepares the RemoteXY system for use.
 */
void setup() {
  pinMode(LED_PIN, OUTPUT); ///< Initialize LED pin as output
  Serial.begin(9600); ///< Start the serial communication for debugging
  mySerial.begin(9600); ///< Start the software serial communication
  RemoteXY_Init(); ///< Initialize RemoteXY library
}

/**
 * @brief Main loop to handle RemoteXY communication and control LED.
 * 
 * This function checks the state of the RemoteXY button and controls
 * the LED accordingly. If the button is pressed, the LED is turned on, 
 * and if it is released, the LED is turned off.
 */
void loop() {
  RemoteXY_Handler(); ///< Handle communication with RemoteXY app

  /**
   * @brief LED control based on button state.
   * 
   * If the button in the RemoteXY app is pressed, the LED is turned on.
   * If the button is not pressed, the LED is turned off.
   */
  if (RemoteXY.button1) {
    digitalWrite(LED_PIN, HIGH); ///< Turn LED on
  } else {
    digitalWrite(LED_PIN, LOW); ///< Turn LED off
  }
}

