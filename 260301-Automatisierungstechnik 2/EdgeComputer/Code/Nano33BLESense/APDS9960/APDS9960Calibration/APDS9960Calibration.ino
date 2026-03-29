// Code to determine the calibration factors for the application `ApplicationAPDS9960.ino`. 
// To start, open the serial monitor and type "OK" into the command line, then press Enter. 
// After reading the information, always confirm with "OK".

// file: APDS9960Calibration.ino

#include <Arduino_APDS9960.h>

// RGB LED Pins for the Arduino Nano 33 BLE Sense
const int redPin = LEDR;
const int greenPin = LEDG;
const int bluePin = LEDB;

// Maximum RGB values based on calibration measurements
int maxRed = 0;
int maxGreen = 0;
int maxBlue = 0;

// Calibration state variables
bool initialOKConfirmed = false;      // Confirmation to start explanation
bool explanationShown = false;        // Flag to show calibration explanation
bool explanationConfirmed = false;    // Confirmation that explanation was read
bool redCalibrated = false;           // Flag for completed red calibration
bool greenCalibrated = false;         // Flag for completed green calibration
bool blueCalibrated = false;          // Flag for completed blue calibration
bool readyForRed = false;             // Flag to begin red calibration
bool readyForGreen = false;           // Flag to begin green calibration
bool readyForBlue = false;            // Flag to begin blue calibration

void setup() {
  Serial.begin(9600);

  // Set up RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Turn on all LEDs (for white light) to capture accurate color
  digitalWrite(redPin, LOW); 
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  // Initialize the APDS-9960 sensor and check for success
  if (!APDS.begin()) {
    Serial.println("Error initializing the APDS-9960 sensor!");
    while (1); // Stop program if sensor fails to initialize
  }

  Serial.println("Sensor successfully initialized.");
  Serial.println("Type 'OK' to proceed to the explanation of the calibration process.");
}

void loop() {
  int r = 0, g = 0, b = 0, a = 0; // Variables to hold RGB and ambient values

  // Initial confirmation to proceed to calibration explanation
  if (!initialOKConfirmed) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove whitespace
      if (input.equalsIgnoreCase("OK")) {
        initialOKConfirmed = true;
        explanationShown = true;
        Serial.println("Explanation:");
        Serial.println("Welcome to the calibration process.");
        Serial.println("You will be prompted to calibrate with the red, green, and blue color charts.");
        Serial.println("Follow the instructions and confirm each step by typing 'OK'.");
        Serial.println("Type 'OK' to begin the calibration process.");
      }
    }
    return; // Exit loop until 'OK' is entered initially
  }

  // Wait for user confirmation after showing explanation
  if (explanationShown && !explanationConfirmed) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove whitespace
      if (input.equalsIgnoreCase("OK")) {
        explanationConfirmed = true;
        readyForRed = true;
        Serial.println("Explanation confirmed.");
        Serial.println("Place the red color chart in front of the sensor and type 'OK' when ready.");
      }
    }
    return; // Exit loop until explanation is confirmed
  }

  // Red calibration step
  if (readyForRed && !redCalibrated) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove whitespace
      if (input.equalsIgnoreCase("OK")) {
        readyForRed = false;
        Serial.println("Measuring red value for 10 seconds...");

        unsigned long startTime = millis(); // Start time for 10-second calibration
        int maxMeasuredRed = 0;

        // Measure red value for 10 seconds
        while (millis() - startTime < 10000) {
          if (APDS.colorAvailable()) {
            APDS.readColor(r, g, b, a);
            if (r > maxMeasuredRed) {
              maxMeasuredRed = r; // Update max red value if current is higher
            }
          }
          delay(100); // Avoid sensor read flooding
        }

        maxRed = maxMeasuredRed; // Set max red after calibration
        redCalibrated = true;
        readyForGreen = true;
        Serial.println("Red calibration completed.");
        Serial.println("Place the green color chart in front of the sensor and type 'OK' when ready.");
      }
    }
    return; // Exit loop until red calibration is confirmed
  }

  // Green calibration step
  if (readyForGreen && !greenCalibrated) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove whitespace
      if (input.equalsIgnoreCase("OK")) {
        readyForGreen = false;
        Serial.println("Measuring green value for 10 seconds...");

        unsigned long startTime = millis(); // Start time for 10-second calibration
        int maxMeasuredGreen = 0;

        // Measure green value for 10 seconds
        while (millis() - startTime < 10000) {
          if (APDS.colorAvailable()) {
            APDS.readColor(r, g, b, a);
            if (g > maxMeasuredGreen) {
              maxMeasuredGreen = g; // Update max green if current is higher
            }
          }
          delay(100); // Avoid sensor read flooding
        }

        maxGreen = maxMeasuredGreen; // Set max green after calibration
        greenCalibrated = true;
        readyForBlue = true;
        Serial.println("Green calibration completed.");
        Serial.println("Place the blue color chart in front of the sensor and type 'OK' when ready.");
      }
    }
    return; // Exit loop until green calibration is confirmed
  }

  // Blue calibration step
  if (readyForBlue && !blueCalibrated) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove whitespace
      if (input.equalsIgnoreCase("OK")) {
        readyForBlue = false;
        Serial.println("Measuring blue value for 10 seconds...");

        unsigned long startTime = millis(); // Start time for 10-second calibration
        int maxMeasuredBlue = 0;

        // Measure blue value for 10 seconds
        while (millis() - startTime < 10000) {
          if (APDS.colorAvailable()) {
            APDS.readColor(r, g, b, a);
            if (b > maxMeasuredBlue) {
              maxMeasuredBlue = b; // Update max blue if current is higher
            }
          }
          delay(100); // Avoid sensor read flooding
        }

        maxBlue = maxMeasuredBlue; // Set max blue after calibration
        blueCalibrated = true;
        Serial.println("Blue calibration completed.");
        Serial.println("Calibration complete.");
        
        // Display final calibration results
        Serial.println("Calibration results:");
        Serial.print("maxRed = ");
        Serial.println(maxRed);
        Serial.print("maxGreen = ");
        Serial.println(maxGreen);
        Serial.print("maxBlue = ");
        Serial.println(maxBlue);
      }
    }
    return; // Exit loop until blue calibration is confirmed
  }
}
