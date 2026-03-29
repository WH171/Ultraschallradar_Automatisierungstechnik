// Code for using the GRV Heart Rate 3 Sensor as a heart rate monitor.
// To calibrate, measure the actual pulse and divide it by the pulse measured by the GRV Heart Rate 3 Sensor.
// Enter this factor as calibrationFactor (line 48).

// Required components: 
// - Arduino Nano 33 BLE Sense
// - GRV Heart Rate 3 Sensor
// - 1.30" IIC OLED Display
// - micro SD card reader with an SD card

// file: ApplicationHeartRate.ino

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <SD.h>

// Include SPI and Wire libraries conditionally if hardware SPI/I2C is available
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Initialize display object for a 128x64 OLED with I2C interface
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ A5, /* data=*/ A4);

// Define sensor and SD card pins
const int heartRatePin = A0;
const int batteryPin = A1;
const int chipSelect = 10;

// Variables for heart rate calculation
int heartRateValue = 0;
int lastHeartRateValue = 0;
unsigned long lastPeakTime = 0;
unsigned long lastHeartbeatTime = 0;
bool peakDetected = false;
int threshold = 512;
int heartRate = 0;
int heartRateArray[10];
int heartRateIndex = 0;
int heartRateSum = 0;
int previousHeartRate = 0;
const unsigned long errorTimeout = 5000; // 5-second timeout for error detection
bool errorState = false;
double calibrationFactor = 1;

// Variables for display switching
unsigned long lastSwitchTime = 0;
const unsigned long switchInterval = 10000; // 10-second interval for display mode toggle
bool showGraph = false;
int graphData[108];

// Flags and timers for SD card and initial setup
bool sdCardFull = false;
unsigned long startTime;
bool initialMeasurement = true;

void setup(void) {
  Serial.begin(9600);
  pinMode(heartRatePin, INPUT);
  pinMode(batteryPin, INPUT);

  u8g2.begin(); // Initialize display

  // SD card initialization
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Display initial message
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  const char* startText1 = "Measurement starting,";
  const char* startText2 = "please wait.";

  int16_t x1 = (128 - u8g2.getStrWidth(startText1)) / 2;
  int16_t x2 = (128 - u8g2.getStrWidth(startText2)) / 2;
  int16_t y1 = 30;
  int16_t y2 = 45;

  u8g2.drawStr(x1, y1, startText1);
  u8g2.drawStr(x2, y2, startText2);

  u8g2.sendBuffer();

  startTime = millis();
}

void loop(void) {
  // Delay measurement start by 5 seconds
  if (millis() - startTime < 5000) {
    return;
  }

  // Stop execution if SD card is full
  if (sdCardFull) {
    return;
  }

  heartRateValue = analogRead(heartRatePin);
  float rawBatteryValue = analogRead(batteryPin);
  float voltage = rawBatteryValue * (3.3 / 1023.0); // Calculate battery voltage

  Serial.print("Measured voltage at A1: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Heartbeat detection based on threshold crossing
  if (heartRateValue > threshold && lastHeartRateValue <= threshold) {
    if (!peakDetected) {
      peakDetected = true;
      unsigned long peakInterval = millis() - lastPeakTime;
      lastPeakTime = millis();
      lastHeartbeatTime = millis();
      errorState = false;
      initialMeasurement = false;
      
      if (peakInterval > 0) {
        heartRate = 60000 / peakInterval; // Calculate heart rate in BPM

        // Update average heart rate
        heartRateSum -= heartRateArray[heartRateIndex];
        heartRateArray[heartRateIndex] = heartRate;
        heartRateSum += heartRate;
        heartRateIndex = (heartRateIndex + 1) % 10;
        int averageHeartRate = (heartRateSum / 10) * calibrationFactor;

        Serial.print("Heart Rate: ");
        Serial.println(averageHeartRate);

        // Update display if heart rate has changed
        if (averageHeartRate != previousHeartRate) {
          previousHeartRate = averageHeartRate;

          if (!showGraph) {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawStr(0, 20, "Heart Rate:");
            u8g2.setFont(u8g2_font_fub25_tr);

            char heartRateStr[10];
            sprintf(heartRateStr, "%d", averageHeartRate);
            int16_t x = (128 - u8g2.getStrWidth(heartRateStr) - u8g2.getStrWidth(" BPM")) / 2;
            u8g2.drawStr(x, 50, heartRateStr);
            u8g2.drawStr(x + u8g2.getStrWidth(heartRateStr), 50, " BPM");

            drawBattery(voltage); // Draw battery level

            u8g2.sendBuffer();
          }
        }

        updateGraph(averageHeartRate); // Update graph data
        saveToSD(averageHeartRate);    // Save data to SD card
      }
    }
  } else if (heartRateValue <= threshold) {
    peakDetected = false;
  }

  // Check for error state if no heartbeat detected within timeout
  if (!initialMeasurement && millis() - lastHeartbeatTime > errorTimeout) {
    errorState = true;
    showGraph = false;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 20, "Heart Rate:");
    u8g2.setFont(u8g2_font_fub25_tr);
    u8g2.drawStr(30, 50, "Error");

    drawBattery(voltage);

    u8g2.sendBuffer();
  } else {
    errorState = false;
  }

  // Toggle display between heart rate and graph every 10 seconds
  if (!errorState && millis() - lastSwitchTime > switchInterval) {
    showGraph = !showGraph;
    lastSwitchTime = millis();
  }

  if (showGraph) {
    drawGraph(); // Draw heart rate graph
  }

  lastHeartRateValue = heartRateValue;
  delay(10);
}

// Display battery level as segmented bars
void drawBattery(float voltage) {
  int batteryLevel = map(voltage * 10, 0, 90, 0, 4);

  u8g2.drawFrame(100, 2, 24, 10);
  u8g2.drawBox(124, 4, 2, 6);

  for (int i = 0; i < batteryLevel; i++) {
    u8g2.drawBox(102 + (i * 5), 4, 4, 6);
  }
}

// Update graph data for real-time heart rate display
void updateGraph(int heartRate) {
  for (int i = 0; i < 107; i++) {
    graphData[i] = graphData[i + 1];
  }
  graphData[107] = (heartRate > 40) ? heartRate : 0;
}

// Draw heart rate graph on the display
void drawGraph() {
  int xOffset = 10;
  int yOffset = 10;

  u8g2.clearBuffer();
  u8g2.drawLine(xOffset, yOffset + 44, xOffset + 108, yOffset + 44);
  u8g2.drawLine(xOffset, yOffset, xOffset, yOffset + 44);

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(xOffset + 60, yOffset + 54 , "Time (s)");
  u8g2.drawStr(xOffset - 10, yOffset - 2, "BPM");

  for (int i = 1; i < 108; i++) {
    int y1 = yOffset + 44 - map(graphData[i - 1], 40, 140, 0, 44);
    int y2 = yOffset + 44 - map(graphData[i], 40, 140, 0, 44);
    if (graphData[i - 1] > 0 && graphData[i] > 0) {
      u8g2.drawLine(xOffset + i - 1, y1, xOffset + i, y2);
    }
  }

  u8g2.sendBuffer();
}

// Save heart rate data to SD card
void saveToSD(int heartRate) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.print("Heart Rate: ");
    dataFile.print(heartRate);
    dataFile.println(" BPM");
    dataFile.close();
    Serial.println("Data saved to SD card.");
  } else {
    Serial.println("Error opening file for writing. SD card may be full.");
    sdCardFull = true;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 20, "SD Card Full!");
    u8g2.sendBuffer();
  }
}
