/**
 * @file ServoManual.ino
 * @brief Manual servo control without library
 * 
 * @details Demonstrates servo motor control through direct PWM generation
 * using digitalWrite() and delayMicroseconds(). The servo is positioned
 * at approximately 90° (center position). This method shows how the
 * Servo library works at the hardware level.
 * 
 * @author Simon Müller
 * @date 01.05.2026
 * @version 1.0
 * 
 * @section hardware Hardware
 * - Arduino Board (e.g. Uno, Nano)
 * - Servo motor connected to Pin 9
 * - Power supply: 5V for servo (VCC and GND)
 * 
 * @section notes Notes
 * This implementation uses blocking delays and is for educational purposes.
 * For production code, use the Arduino Servo library instead.
 */

int servoPin = 9;  ///< Pin number for servo control signal

/**
 * @brief Initialization at program startup
 * 
 * @details Configures the servo pin as OUTPUT to enable
 * sending PWM signals to the servo motor.
 * 
 * @return void
 */
void setup() {
    pinMode(servoPin, OUTPUT);  // Set servo pin as output
}

/**
 * @brief Main loop for continuous PWM generation
 * 
 * @details Manually generates a 50 Hz PWM signal (20 ms period).
 * The HIGH pulse of 1.5 ms corresponds to a servo position of approx. 90°.
 * 
 * PWM Timing:
 * - HIGH phase: 1500 µs (1.5 ms) → 90° position (center)
 * - LOW phase: 18500 µs (18.5 ms) → fills up to 20 ms total period
 * - Frequency: 50 Hz (1000000 µs / 20000 µs = 50 Hz)
 * 
 * @note This method blocks program execution. For more complex
 * applications, the Servo library should be used.
 * 
 * @return void
 */
void loop() {
    // Move to approx. 90 degrees (1.5ms HIGH pulse)
    digitalWrite(servoPin, HIGH);    ///< Starts the HIGH pulse
    delayMicroseconds(1500);         ///< Pulse width: 1.5 ms → 90° position
    digitalWrite(servoPin, LOW);     ///< Ends the HIGH pulse
    
    // Fill the remaining 20ms period (50 Hz)
    delayMicroseconds(18500);        ///< LOW phase: 18.5 ms until next period
}