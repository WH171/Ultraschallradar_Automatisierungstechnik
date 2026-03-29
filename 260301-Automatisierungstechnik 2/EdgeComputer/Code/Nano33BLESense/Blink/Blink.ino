/**
 * @file blink.ino
 * @brief Simple example to blink the on-board LED on and off.
 * 
 * @details
 * Turns the built-in LED on for one second, then off for one second repeatedly.
 * Most Arduino boards have a built-in LED connected to a specific digital pin.
 * The macro LED_BUILTIN is used to ensure compatibility across different boards.
 * 
 * @note For more details about your board's built-in LED, see:
 *       https://www.arduino.cc/en/Main/Products
 * 
 * @see https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
 * 
 * @author
 * - Scott Fitzgerald (original)
 * - Arturo Guadalupi (modification)
 * - Colby Newman (modification)
 * 
 * @date Modified:
 * - 8 May 2014
 * - 2 Sep 2016
 * - 8 Sep 2016
 * 
 * @copyright Public Domain
 */

/**
 * @brief Initializes the LED pin as an output.
 * 
 * This function runs once at startup after reset or power-up.
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  ///< Set the LED pin as output
}

/**
 * @brief Toggles the LED on and off every second.
 * 
 * This function runs repeatedly after setup().
 */
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  ///< Turn the LED on
  delay(1000);                      ///< Wait for one second
  digitalWrite(LED_BUILTIN, LOW);   ///< Turn the LED off
  delay(1000);                      ///< Wait for one second
}
