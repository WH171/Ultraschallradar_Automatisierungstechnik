#include <LED.h>
#include <LEDBuiltin.h>
#include <LEDPower.h>
#include <SignsOfLife.h>




/**
*
*  @file TestLEDPower.h
*
*  @brief header file for the library for using  the Power LED of an Arduino
*	
*  ...
*  
*/


/** 
 * @brief Creates a PowerLED object for controlling the power LED on the board.
 * 
 * This object controls the onboard power LED connected to a predefined pin (usually pin 25).
 */
LEDPower LEDPower; // Power-LED on pin 25

//SignsOfLife signs(13, 500, 500); // Blink an LED on pin 13 with 500ms on and off times

/**
 * @brief Setup function runs once when the sketch starts.
 * 
 * Initializes the power LED for use in the main loop. The pin for the LED is configured here.
 */
void setup() {
  LEDPower.setup();  ///< Initialize the power LED (usually pin 25 on the Nano 33 BLE Sense)
}

/**
 * @brief Main loop function runs repeatedly after setup.
 * 
 * This function turns the power LED on for 2 seconds, then off for 500ms.
 */
void loop() {
  LEDPower.turnOn();  ///< Turns on the power LED
  delay(2000);        ///< Wait for 2 seconds (LED is ON)

  LEDPower.turnOff(); ///< Turns off the power LED
  delay(500);         ///< Wait for 500 milliseconds (LED is OFF)
}