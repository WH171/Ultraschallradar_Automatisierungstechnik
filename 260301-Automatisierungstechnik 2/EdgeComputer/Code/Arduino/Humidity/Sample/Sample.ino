/**
* @file SimpleSensorRead.ino
* @brief Example code for reading a soil moisture sensor. 
* 
* This example code continuously reads analog values from 
* the moisture sensor and outputs them via the serial interface.  
* It is intended as a basic test to verify that the sensor is working properly.
*
* @author S. Goekalp 
* @date 2025-04-20
*/

/// Pin to which the moisture sensor is connected 
const int sensorPin = A6; ///< Sensor is connected to analog pin A0

/**
* @brief Setup function - runs once at startup.
* Initializes serial communication for outputting sensor data. 
*/

void setup () {
  Serial.begin(9600); ///< Start serial communication at 9600 baud, 9600 baud = 9600 symbols/bits per second (standard rate)
}

/**
*@brief Main loop for continuous sensor reading.
*
*This loop regularly reads the sensor value and prints it to the seriale console. 
*/

void loop() {
  int sensorValue = analogRead (sensorPin); ///< Read analog value from the moisture 

  Serial.print ("Sensorwert: ");
  Serial.print (sensorValue); ///< Output the value to the serial console
  delay (1000);               ///< Wait 1 second before the next measurement
}
