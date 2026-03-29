/**
 * @file ManualHardwareSerialBib.ino
 * @brief Demonstrates communication using HardwareSerial.
 *
 * This example shows how to use the built-in HardwareSerial interface 
 * (e.g., Serial1) to communicate with an external serial device.
 * Make sure your board supports multiple hardware UART ports (e.g., Arduino Mega, ESP32).
 */

#define SERIAL_HW Serial1  ///< Define the hardware serial port to be used

/**
 * @brief Initializes the hardware serial interface and sends a welcome message.
 *
 * This function is called once at startup. It initializes Serial1 with a baud rate 
 * of 9600 and sends a message to the connected serial device to confirm that 
 * communication has been established. It also opens the USB serial monitor (Serial)
 * for debugging purposes.
 */
void setup() {
  // Start hardware serial communication
  SERIAL_HW.begin(9600);

  // Start USB serial monitor
  Serial.begin(9600);
  while (!Serial); // Wait for monitor connection (only necessary on some boards)

  // Send initialization message
  SERIAL_HW.println("HardwareSerial active!");
  Serial.println("HardwareSerial initialized.");
}

/**
 * @brief Continuously reads incoming data, echoes it, and transmits messages.
 *
 * In the loop function, the program checks for incoming characters on Serial1.
 * If data is received, it is read and echoed back to the sender. A status message
 * is also printed every second. This can be used for basic testing or device communication.
 */
void loop() {
  // Check if data is available
  if (SERIAL_HW.available() > 0) {
    // Read one character
    char incoming = SERIAL_HW.read();

    // Echo the received character
    SERIAL_HW.print("Received: ");
    SERIAL_HW.println(incoming);

    // Also print to USB serial monitor
    Serial.print("Received over HardwareSerial: ");
    Serial.println(incoming);
  }

  // Periodically send a test message
  SERIAL_HW.println("Message: Hello, how are you?");

  // Wait for a second
  delay(1000);
}
