/**
 * @file grove_mosfet_solenoid.ino
 * @brief Demo to control a solenoid using the Grove - MOSFET module with digital output.
 *
 * This sketch demonstrates how to control a solenoid (e.g. an electromagnetic actuator)
 * using a Grove - MOSFET module connected to digital pin D12. The solenoid alternates 
 * between activated and deactivated states in a timed loop using digitalWrite().
 *
 * @author Seeed Studio (adapted)
 * @version 1.1
 * @date 2025
 */

/// Digital pin connected to the Grove - MOSFET SIG pin (controls the solenoid)
const int solenoidPin = 12;

/**
 * @brief Arduino setup function.
 *
 * Initializes the serial interface and sets the solenoid control pin as output.
 */
void setup()
{
    Serial.begin(38400);
    pinMode(solenoidPin, OUTPUT);
    Serial.println("Grove - MOSFET Solenoid Demo!");
}

/**
 * @brief Arduino loop function.
 *
 * Repeatedly activates and deactivates the solenoid with a delay between states.
 */
void loop()
{
    activateSolenoid();
    delay(2000); // Solenoid on for 2 seconds

    deactivateSolenoid();
    delay(2000); // Solenoid off for 2 seconds
}

/**
 * @brief Activates the solenoid by setting the control pin HIGH.
 */
void activateSolenoid()
{
    digitalWrite(solenoidPin, HIGH);
    Serial.println("Solenoid ON");
}

/**
 * @brief Deactivates the solenoid by setting the control pin LOW.
 */
void deactivateSolenoid()
{
    digitalWrite(solenoidPin, LOW);
    Serial.println("Solenoid OFF");
}
