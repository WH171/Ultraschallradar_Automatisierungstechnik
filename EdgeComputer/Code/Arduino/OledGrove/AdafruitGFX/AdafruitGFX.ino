#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @file AdafruitGFX-Example.ino
 * @brief Initializes an SSD1306 OLED display and prints a simple message.
 * 
 * This sketch demonstrates how to initialize a 128x64 OLED display
 * and render basic text using the Adafruit GFX and SSD1306 libraries.
 */

Adafruit_SSD1306 display(128, 64, &Wire, -1);

/**
 * @brief Initializes the OLED display and shows a default-sized message.
 * 
 * Sets up the display, clears the screen, configures text size and color,
 * and prints a simple "Hello World" message at the top-left corner.
 */
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); ///< Initialize display with power source
  display.clearDisplay();                    ///< Clear the display buffer
  display.setTextSize(1);                    ///< Set text size to default (1x)
  display.setTextColor(WHITE);               ///< Set text color to white
  display.setCursor(0, 0);                   ///< Position cursor at top-left corner
  display.println("Hello World");            ///< Print message to buffer
  display.display();                         ///< Render buffer to screen
}