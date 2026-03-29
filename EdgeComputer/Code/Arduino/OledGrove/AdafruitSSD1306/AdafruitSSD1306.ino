/**
 * @AdafruitSSD1306-Example.ino
 * @brief Initializes an OLED display using Adafruit_SSD1306 and displays a startup message.
 *
 * This code uses the Adafruit_GFX and Adafruit_SSD1306 libraries to interface with a 128x64 OLED display
 * over I2C. Upon initialization, it clears the screen and displays the word "Start" in large font.
 */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

/**
 * @brief Initializes the OLED display and prints the word "Start" in large font.
 */
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(10, 10);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Start");
  display.display();
}
