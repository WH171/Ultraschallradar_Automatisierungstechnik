// How to control the power LED of the Arduino PortentaH7
// File: TestLEDPowerBrightness.ino
// Using PWM, the LED's brightness is increasing until full and reverse.

#define LED_PWR 25 // Define the pin for the power LED

// Define the initial brightness values (0-255)
int Brightness = 0;
// Define the increment/decrement value
int Step = 5;

void setup() {
  // Initialize the pin as an output
  pinMode(LED_PWR, OUTPUT);
}

void loop() {
  // Write the PWM values to the LED pin
  analogWrite(LED_PWR, Brightness);
  // Update the brightness values
  Brightness += Step;

  // Check if the brightness values are out of range and reverse the direction
  if (Brightness <= 0 || Brightness >= 255) {
    Step = -Step;
  }

  // Wait for 10 milliseconds
  delay(10);
}
