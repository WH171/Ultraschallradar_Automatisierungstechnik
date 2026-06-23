/**
 * @file HelloWorld.ino
 * @brief Minimal example for initializing the serial interface
 *        and printing a test message.
 * @details This program demonstrates the use of Serial.begin()
 *          and Serial.println() on the Arduino Nano 33 BLE Sense.
 *          After the microcontroller starts, the serial interface
 *          is initialized and the string "Hello World" is printed
 *          once to the serial monitor.
 * @author Simon Müller
 * @date 2026-06-23
 */


/**
 * @brief Initializes the serial interface and sends a test message.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; ///< Wait until the serial interface is ready.
  }
  Serial.println("Hello World");
}


/**
 * @brief Contains the cyclic part of the program.
 * @details In this example, the main loop is empty because no
 *          recurring processing is required.
 */
void loop() {
  // no recurring processing
}