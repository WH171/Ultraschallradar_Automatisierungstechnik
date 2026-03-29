/**
 * @file TestOLEDDisplay.ino
 * 
 * @brief Basic example for initializing and using an SSD1306 OLED display with the Arduino Nano 33 BLE Sense.
 * @date 2025-04-14
 * @author Wedemann
 * @details 
 * This sketch demonstrates how to initialize the OLED screen and display a simple text message.
 * The display is then set to repeatedly invert its pixels to create a blinking effect.
 * 
 * Libraries used:
 * - Wire.h: For I2C communication.
 * - Adafruit_GFX.h Version 1.12.0: Core graphics library for Adafruit displays. 
 * - Adafruit_SSD1306.h Version 2.5.13: Driver for SSD1306-based OLED displays.
 * 
 * Hardware:
 * - Arduino Nano 33 BLE Sense
 * - SSD1306 OLED display (128x64 pixels) connected via I2C.
 * 
 * Setup:
 * - SDA to A4 (or as defined by your board)
 * - SCL to A5
 * - Power and ground accordingly
 * 
 * Usage:
 * - The OLED displays "Hello, world!" upon startup.
 * - The display inverts (blinks) every second within the loop.
 * 
 * Notes:
 * - Ensure the OLED is correctly connected and powered.
 * - The display I2C address is set to 0x3C (common default).
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 ///< OLED display width in pixels
#define SCREEN_HEIGHT 64 ///< OLED display height in pixels

// Create display object for SSD1306 using I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication, sets up the OLED display, and prints a static message.
 */
void setup() {
  Serial.begin(9600); ///< Start serial communication at 9600 baud

  // Attempt to initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is a common I2C address for 128x64 OLEDs
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); ///< Infinite loop if initialization fails
  }

  delay(2000); ///< Short delay to allow the display to initialize

  display.clearDisplay(); ///< Clear any previous display content

  display.setTextSize(1); ///< Set text size to smallest
  display.setTextColor(WHITE); ///< Set text color to white
  display.setCursor(0, 10); ///< Position text cursor
  display.println("Hello, world!"); ///< Display a static greeting
  display.display(); ///< Update the screen with written content
}

/**
 * @brief Arduino main loop function.
 * 
 * Causes the OLED display to blink by inverting and restoring pixels every second.
 */
void loop() {
  display.invertDisplay(true); ///< Invert the display (white -> black, black -> white)
  delay(1000); ///< Wait 1 second
  display.invertDisplay(false); ///< Restore the display to normal
  delay(1000); ///< Wait 1 second
}
