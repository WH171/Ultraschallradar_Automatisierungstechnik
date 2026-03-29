/**
 * @file TestApplicationMosfet.ino
 * 
 * @brief Controls a MOSFET via PWM using a rotary encoder as input.
 * @date 2025-04-14
 * @author Wedemann
 * @details 
 * This sketch reads an incremental rotary encoder and uses it to adjust the PWM duty cycle
 * output to a MOSFET gate. The MOSFET can be used to control motor speed, LED brightness, etc.
 * 
 * Libraries used:
 * - None (relies on built-in Arduino functions)
 * 
 * Hardware:
 * - Arduino Nano 33 BLE Sense
 * - Rotary encoder with pins A and B
 * - Logic-level MOSFET with gate connected to a PWM-capable pin
 * 
 * Setup:
 * - Encoder A to digital pin 2 (interrupt-capable)
 * - Encoder B to digital pin 3
 * - MOSFET gate to pin 9 (PWM output)
 * - Ensure external pull-down on the MOSFET gate if needed
 * 
 * Usage:
 * - Rotate encoder to increase or decrease PWM output (0-255).
 * - PWM signal controls the MOSFET output power.
 * 
 * Notes:
 * - Debouncing may be required depending on encoder quality.
 * - PWM resolution is 8-bit (0-255).
 */

int encoderPinA = 2;              ///< Encoder pin A
int encoderPinB = 3;              ///< Encoder pin B
int mosfetPin = 9;                ///< PWM output to MOSFET gate
int pwmValue = 128;               ///< Initial PWM value (range 0-255)
int encoderPinALast = LOW;        ///< Last state of encoder A
int encoderPinANow = LOW;         ///< Current state of encoder A

/**
 * @brief Arduino setup function.
 * 
 * Initializes encoder inputs, PWM output, and Serial Monitor.
 */
void setup() {
  pinMode(encoderPinA, INPUT_PULLUP); ///< Set encoder A with pull-up
  pinMode(encoderPinB, INPUT_PULLUP); ///< Set encoder B with pull-up
  pinMode(mosfetPin, OUTPUT);         ///< Set PWM pin as output
  Serial.begin(115200);               ///< Start serial communication
}

/**
 * @brief Arduino main loop function.
 * 
 * Reads encoder rotation and updates PWM output to MOSFET.
 */
void loop() {
  encoderPinANow = digitalRead(encoderPinA);

  // Detect falling edge
  if ((encoderPinALast == HIGH) && (encoderPinANow == LOW)) {
    if (digitalRead(encoderPinB) == HIGH) {
      pwmValue += 5; ///< Increase PWM
    } else {
      pwmValue -= 5; ///< Decrease PWM
    }

    // Constrain PWM to valid range
    pwmValue = constrain(pwmValue, 0, 255);

    // Output to MOSFET
    analogWrite(mosfetPin, pwmValue);

    // Debug print
    Serial.print("PWM Value: ");
    Serial.println(pwmValue);
  }

  encoderPinALast = encoderPinANow;
}
