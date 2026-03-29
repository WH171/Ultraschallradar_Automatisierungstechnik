// Code for using the APDS-9960 sensor as a color recognition machine.

// An Arduino Nano 33 BLE Sense and a 1.30" IIC OLED display are required.
// In addition, the calibration factors maxRed, maxGreen, and maxBlue should
// be determined in advance using APDS9960Calibration.ino.

// file: ApplicationAPDS9960.ino

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino_APDS9960.h>

#define I2C_ADDRESS 0x3C  // I2C address for the OLED display
const int LedPin = 12;    // LED pin number

// Calibration values obtained from the calibration program
const int maxRed = 31;   // Maximum calibrated red value
const int maxGreen = 15; // Maximum calibrated green value
const int maxBlue = 13;  // Maximum calibrated blue value

// Initialize display object for a 128x64 OLED with I2C interface
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() 
{
  Serial.begin(9600);  // Start serial communication
  pinMode(LedPin, OUTPUT);  // Set the LED pin as an output

  // Initialize APDS9960 sensor, log error if initialization fails
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }

  // Initialize OLED display and display an "INITIALIZING" message
  oled.begin();
  oled.clearBuffer();  
  oled.setFont(u8g2_font_ncenB08_tr); // Set display font
  oled.drawStr(10, 10, "INITIALIZING!");
  oled.sendBuffer(); // Send the buffer to display

  // Blink LED three times as a visual indication that setup is complete
  for (int counter = 0; counter < 3; counter++) {
    digitalWrite(LedPin, HIGH);
    delay(200);
    digitalWrite(LedPin, LOW);
    delay(200);
  }
  oled.clearBuffer();  // Clear the display buffer after setup
  oled.sendBuffer();
}

void loop() 
{
  // Display "READY" message
  oled.clearBuffer();
  oled.setCursor(10, 20);
  oled.print("READY");
  oled.sendBuffer();

  int timeout = 1000;  // Timeout value for waiting for color data
  // Wait until color data is available or timeout occurs
  while (!APDS.colorAvailable() && timeout > 0) {  
    delay(5);
    timeout -= 5;
  }
  if (timeout <= 0) {
    Serial.println("Color measurement timeout");
    return;
  }

  timeout = 1000;  // Reset timeout for proximity data
  // Wait until proximity data is available or timeout occurs
  while (!APDS.proximityAvailable() && timeout > 0) {  
    delay(5);
    timeout -= 5;
  }
  if (timeout <= 0) {
    Serial.println("Proximity measurement timeout");
    return;
  }

  int r, g, b;
  int proximity = APDS.readProximity();  // Read proximity value

  if (proximity != -1) {
    Serial.print("Proximity: ");
    Serial.println(proximity);

    if (proximity < 20) {  // If proximity is below threshold
      oled.clearBuffer();
      digitalWrite(LedPin, HIGH);  // Turn on LED as visual indication

      // Turn on all LEDs (for white light) to capture accurate color
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW);

      // Average multiple color readings for accuracy
      int r_total = 0, g_total = 0, b_total = 0;
      for (int i = 0; i < 5; i++) {
        APDS.readColor(r, g, b);
        r_total += r;
        g_total += g;
        b_total += b;
        delay(750);  // Delay between readings
      }
      r = r_total / 5;
      g = g_total / 5;
      b = b_total / 5;

      // Map color values to a 0-255 range using calibration values
      r = map(r, 0, maxRed, 0, 255);
      g = map(g, 0, maxGreen, 0, 255);
      b = map(b, 0, maxBlue, 0, 255);

      // Constrain values to the 0-255 range
      r = constrain(r, 0, 255);
      g = constrain(g, 0, 255);
      b = constrain(b, 0, 255);

      // Print color values for debugging
      Serial.print("Red light = ");  
      Serial.println(r);
      Serial.print("Green light = ");
      Serial.println(g);
      Serial.print("Blue light = ");
      Serial.println(b);
      Serial.println();

      // Determine the dominant color and display result on OLED
      oled.setCursor(10, 20);
      if (r > g && r > b) {
        oled.print("The object is red.");
      } else if (g > r && g > b) {
        oled.print("The object is green.");
      } else if (b > g && b > r) {
        oled.print("The object is blue.");
      }

      oled.sendBuffer(); // Display the color information
      delay(4000);
      oled.clearBuffer();
      oled.sendBuffer();
    } else {
      // Display message if no object is found within proximity threshold
      oled.clearBuffer();
      oled.setCursor(10, 20);
      oled.print("No object found.");
      oled.setCursor(10, 30);
      oled.print("Hold an object");
      oled.setCursor(10, 40);
      oled.print("in front of sensor");
      oled.setCursor(10, 50);
      oled.print("or change position.");
      oled.sendBuffer();
      delay(4000);
      oled.clearBuffer();
      oled.sendBuffer();
    }
  } else {
    // Display error if proximity reading fails
    oled.clearBuffer();
    oled.setCursor(10, 20);
    oled.print("Proximity failed!");
    oled.sendBuffer();
  }

  // Turn off LED and set RGB LEDs to off state after processing
  digitalWrite(LedPin, LOW);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}
