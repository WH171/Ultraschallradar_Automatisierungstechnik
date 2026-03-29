/**
* @file TestBattery.ino
* @author Wings
* @date 20.05.2024
* @details The sketch is used to test the voltage sensor. For the test, the voltage is measured on a 9 V block battery and output on the serial monitor.
*
**/

/** Pin A7 is referenced as the voltage pin for the voltage sensor. */
#define VoltagePin A7

/** The parameter SignalEingang is later assigned the value read in from the VoltagePin pin. */
long SignalEingang;

/** The  parameter Spannung stands for the voltage that is measured with the voltage sensor and has the unit volt. */
double Voltage;

/**
* @brief Starting serial communication
*
* @details The function is executed when the programme is started. The serial interface to the computer is initialised at 9600 baud 
* There is no return value.
*/
void setup() {
  Serial.begin(9600);
}

/**
* @brief Display of the measured voltage
*
* @details The function is executed continuously. The signal at the pin VoltagePin is read out and a voltage value is converted.
* The value read in at the voltage pin is between 0 and 1023. The voltage measurement range is between 0 and 16.5 V. The read-in value can be converted into a voltage using the function map().
* The voltage is displayed in volts on the serial monitor. There is a 5-second wait after each cycle to avoid flooding the serial monitor. There is no return value.
*/
void loop() {
  SignalEingang = analogRead(VoltagePin);
  Voltage       = map(SignalEingang, 0, 1023, 0, 165); // Converting the input signal 165 for 16.5 V
  Voltage       = Voltage/10; //  Converting the input signal 165 for 16.5 V
  Serial.print(Voltage);
  Serial.println(" V");
  delay(5000);
}
