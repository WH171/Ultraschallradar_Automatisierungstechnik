/**
 * @file    Servomotor.ino
 * @author  Simon Müller
 * @date    2026-05-01
 * @version 1.0
 *
 * @brief   Servo-based radar scanning system.
 *
 * This sketch controls a TowerPro SG90 (or similar) servo to perform
 * a continuous back-and-forth sweep from 0 deg to 180 deg and back.
 * All position commands are absolute angles
 * (0–180 deg range).
 */

#include <Servo.h>

Servo radarServo;   ///< Servo object used for radar scanning
int pos = 0;        ///< Current servo position in degrees (0–180, absolute)

/**
 * @brief Initializes the servo on startup.
 *
 * Attaches the servo object to digital pin 9 and prepares it
 * for subsequent position commands. The servo will interpret
 * all following write() calls as absolute target angles in
 * the range 0°–180°.
 */
void setup() {
    radarServo.attach(9);  // Attach servo to pin 9
}

/**
 * @brief Main loop for continuous servo motion.
 *
 * The servo is moved in two phases using absolute angle commands:
 * - Phase 1: Sweep from 0° to 180° in 1° steps
 * - Phase 2: Sweep from 180° back to 0° in 1° steps
 *
 * Each call to radarServo::write(pos) sets an absolute target
 * angle; it does NOT move the servo by a relative offset.
 * With a 15 ms delay between steps, one complete cycle
 * takes approximately 5.4 seconds.
 */
void loop() {
    // Phase 1: sweep from 0° to 180° (absolute positions)
    for (pos = 0; pos <= 180; pos += 1) {
        radarServo.write(pos);  ///< Set servo to absolute angle 'pos'
        delay(15);              ///< Wait 15 ms to reach the position
    }

    // Phase 2: sweep from 180° back to 0° (absolute positions)
    for (pos = 180; pos >= 0; pos -= 1) {
        radarServo.write(pos);  ///< Set servo to absolute angle 'pos'
        delay(15);              ///< Wait 15 ms
    }
}