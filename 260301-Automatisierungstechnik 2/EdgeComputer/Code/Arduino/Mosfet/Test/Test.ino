/**
 * @brief Simple MOSFET control example using an LED
 * Turns an LED on and off using a MOSFET and Arduino pin.
 */

#define MOSFET_GATE_PIN 3

void setup() {
  pinMode(MOSFET_GATE_PIN, OUTPUT);
}

void loop() {
  digitalWrite(MOSFET_GATE_PIN, LOW);  // For P-Kanal MOSFET: Turn ON
  delay(1000);                         // LED ON for 1 second

  digitalWrite(MOSFET_GATE_PIN, HIGH); // Turn OFF
  delay(1000);                         // LED OFF for 1 second
}