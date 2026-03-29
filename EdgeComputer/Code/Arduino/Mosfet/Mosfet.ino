/**
 * @file TestMosfet.ino
 * 
 * @brief Simple MOSFET test using PWM signal on Arduino Nano 33 BLE Sense.
 * @date 2025-04-14
 * @author Wedemann
 * @details 
 * This test program sets a fixed PWM value on a digital pin connected to the gate of a logic-level MOSFET.
 * The goal is to verify that the MOSFET switches properly and delivers power to a connected load (e.g. LED, motor).
 * 
 * Libraries used:
 * - None (uses built-in Arduino functions)
 * 
 * Hardware:
 * - Arduino Nano 33 BLE Sense
 * - Logic-level MOSFET
 * - Load (e.g., LED + resistor or motor)
 * 
 * Setup:
 * - Gate of MOSFET connected to pin 9 (PWM capable)
 * - Source to GND
 * - Drain to one side of the load; the other side of the load to V+
 * - Use an external pull-down resistor on the gate if necessary
 * 
 * Usage:
 * - Upload this sketch to the board
 * - The MOSFET receives a ~50% PWM signal (127/255)
 * - If working, the connected load should be powered with reduced intensity
 * 
 * Notes:
 * - No input interaction; static PWM signal
 * - Change the value in analogWrite() to test different duty cycles
 */

int mosfetPin = 9; ///< PWM output pin connected to MOSFET gate

/**
 * @brief Arduino setup function.
 * 
 * Sets the MOSFET pin as output.
 */
void setup() {
  pinMode(mosfetPin, OUTPUT); ///< Configure PWM pin as output
}

/**
 * @brief Arduino main loop function.
 * 
 * Applies a constant PWM signal (~50% duty cycle) to the MOSFET.
 */
void loop() {
  analogWrite(mosfetPin, 127); ///< Send fixed PWM signal to gate
}
