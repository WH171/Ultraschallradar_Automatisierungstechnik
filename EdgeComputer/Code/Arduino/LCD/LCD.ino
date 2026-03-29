#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>             // SPI library for communication

// Pin definitions for Pico4ML-BLE (RP2040)
#define TFT_CS    12  // Chip Select (GP9)
#define TFT_DC    11  // Data/Command (GP8)
#define TFT_RST   10  // Reset (GP7)
#define TFT_BL    9   // Backlight (GP6, optional)

// SPI pins are predefined for the RP2040:
// MOSI (SDA) -> GP11 (Arduino pin 15)
// SCK (SCL)  -> GP10 (Arduino pin 14)
// MISO is not used by the ST7735

// Initialize the ST7735 display
// Use SPI hardware interface (SPI1 on RP2040)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Initialize serial for debugging (optional)
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (optional, can remove if not using Serial)
  }

  // Initialize the backlight pin (optional)
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Turn on backlight

  // Initialize the display
  // Use the ST7735_160x80 configuration (you may need to adjust based on your display's init sequence)
  tft.initR(INITR_144GREENTAB); // Initialize for 1.44" display (closest match for 160x80)
  
  // Adjust the display offset and size for 160x80 resolution
  tft.setRotation(1); // Rotate to landscape (160x80)
  tft.setTextWrap(false); // Disable text wrapping

  // Clear the screen (fill with black)
  tft.fillScreen(ST7735_BLACK);

  // Set text color and size
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1); // 1x scale (smallest readable size)

  // Draw the text "Magic Wand"
  tft.setCursor(10, 10); // Set cursor position (x, y)
  tft.println("Magic Wand");

  // Draw a red rectangle
  tft.drawRect(10, 30, 50, 20, ST7735_RED); // x, y, width, height, color

  // Optionally, fill the rectangle with blue
  tft.fillRect(12, 32, 46, 16, ST7735_BLUE); // Slightly smaller to show the border
}

void loop() {
  // Nothing to do in loop for this example
  // You can add animations or updates here if needed
  delay(1000); // Just to keep the loop from running too fast
}