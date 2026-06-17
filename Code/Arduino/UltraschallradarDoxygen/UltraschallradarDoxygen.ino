/**
 * @file RadarControl.ino
 * @brief Firmware for an automated 120-degree ultrasonic radar system.
 * @details This software controls an ultrasonic radar system consisting of an 
 * Arduino Nano V3, an HC-SR04 ultrasonic sensor, and an SG90 positional servo motor.
 * Upon boot, a dual-LED power-on self-test (POST) is initiated. The hardware waits 
 * for a serial handshake signal ("P:START") from the Processing frontend before 
 * slowly centering the servo to 90°. After a successful self-test, it continuously 
 * scans from 30° to 150°, streaming encoded distance strings via UART.
 * * @author Hinrichs
 * @date 2026-06-17
 * @version 1.3
 */

#include <Servo.h>

/* --- PIN DEFINITIONS --- */
const int PIN_LED_GREEN = 2;   ///< GPIO pin for the green status LED (System Ready / Active)
const int PIN_LED_RED   = 3;   ///< GPIO pin for the red status LED (Error / Awaiting Software)
const int PIN_TRIG      = 4;   ///< Digital output pin to trigger the ultrasonic burst
const int PIN_ECHO      = 5;   ///< Digital input pin to measure the echo pulse travel time
const int PIN_SERVO     = 9;   ///< PWM output pin for precise positional control of the SG90 servo

/* --- GLOBAL OBJECTS --- */
Servo radarServo;              ///< Instance of the Arduino Servo library for axis positioning.

/**
 * @brief Initializes peripheral hardware, handles the handshake protocol, and executes POST.
 * @details Configures UART parameters to 9600 baud and defines GPIO modes. Immediately turns 
 * on the red LED to indicate that power is on but the software frontend is not yet connected. 
 * Once the "P:START" token is received, the red and green LEDs turn on together, the servo slowly 
 * centers to 90°, and a test measurement is taken. If valid (0 - 400 cm), it switches to normal 
 * operation (green LED only); otherwise, it traps execution in an error state.
 */
void setup() {
  // Initialize hardware serial at standard baud rate
  Serial.begin(9600);
  
  // Set I/O pin directions
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // STATE 1: Hardware powered but Processing is closed -> Permanent RED LED indication
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
  
  // HANDSHAKE LOOP: Block execution until Processing initiates connection via "P:START"
  while (true) {
    if (Serial.available() > 0) {
      String msg = Serial.readStringUntil('\n');
      if (msg.indexOf("P:START") != -1) {
        break; // Connection established! Escape loop.
      }
    }
    delay(100);
  }
  
  // STATE 2: SELF-TEST STARTED -> Both LEDs turned ON to indicate POST routine
  Serial.println("S:TESTING");
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  
  radarServo.attach(PIN_SERVO);
  
  // SOFT STARTUP: Slowly sweep servo from current read position to center (90°) to protect gears
  int startAngle = radarServo.read();
  if(startAngle < 0 || startAngle > 180) startAngle = 30; // Fallback routine if unreadable
  
  if (startWinkel < 90) {
    for (int w = startAngle; w <= 90; w++) {
      radarServo.write(w);
      delay(25); // Controls sweep velocity
    }
  } else {
    for (int w = startAngle; w >= 90; w--) {
      radarServo.write(w);
      delay(25);
    }
  }
  delay(500); // Allow physical dampening to settle
  
  // Trigger single ultrasonic test ping (10-microsecond high pulse)
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Read echo duration with a 30ms timeout (approx. 5 meters limit)
  long duration = pulseIn(PIN_ECHO, HIGH, 30000); 
  float distance = duration * 0.0343 / 2.0;
  
  // Evaluate diagnostic signal response
  if (distance > 0 && distance < 400) {
    // STATE 3: DIAGNOSTICS PASSED -> Green LED ON, Red LED OFF -> Begin normal loops
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
    Serial.println("S:OK");
  } else {
    // SENSOR CRITICAL FAULT -> Force RED LED on and trap hardware
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    Serial.println("S:ERR_SENSOR");
    while(1); // Dead-loop prevents main execution loops upon device failure
  }
}

/**
 * @brief Continuous main execution loop controlling the sweep profile.
 * @details Drives the servo in a symmetrical 120-degree window centered around 90°.
 * Divided into three linear sweeps:
 * 1. Mid-to-left phase (90° down to 30°).
 * 2. Left-to-right full phase (30° up to 150°).
 * 3. Right-to-mid recovery phase (150° down to 90°).
 */
void loop() {
  for (int angle = 90; angle >= 30; angle--) { scanPosition(angle); }
  for (int angle = 30; angle <= 150; angle++) { scanPosition(angle); }
  for (int angle = 150; angle >= 90; angle--) { scanPosition(angle); }
}

/**
 * @brief Commands the servo to an angle, executes a ping, and streams data.
 * @details Encapsulates the entire data collection cycle for an angular slice:
 * 1. Servo dispatches positional pulse. A 35ms mechanical settle window reduces micro-vibrations.
 * 2. HC-SR04 emits ultrasound burst.
 * 3. Distance is calculated via Time-of-Flight math using the speed of sound ($343\text{ m/s}$).
 * 4. Serial stream broadcasts formatted packet string `D:[Angle],[Distance]` over UART.
 * * @param angle The exact target servo coordinate in degrees (30° to 150°).
 */
void scanPosition(int angle) {
  radarServo.write(angle);
  delay(35); // Necessary settle interval to prevent graphic sweep jitter
  
  // Ultrasonic pulse cycle
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // Capture ToF reply pulse
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  int distance = duration * 0.0343 / 2.0;
  
  // Synchronized telemetry export (e.g., "D:90,24")
  Serial.print("D:");
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);
}