/**
* @file pumptest.ino
* @brief tests pump functionality
*
* This code turns a pump connected to digital pin 8 ON an OFF every
* 3 seconds. It is intended to verify that the pump is wired correctly
* and responds to digital signals.
*
* @author S. Goekalp
* @date 2025-05-20
*/

/// Digital pin to which the pump is connected 
const int pumpPin = 8;

/**
* @brief initializes serial communication and pump pin mode
*/
void setup() {
  Serial.begin(9600);        ///< start serial communication at 9600 baud
  pinMode(pumpPin, OUTPUT)   ///< set pump pin as output 

  Serial.println("Pump test started.");
}

/**
* @brief repeatedly turns the pump ON and OFF every 3 seconds.
*
* This loop toggles the pump state to verify basic operation.
*/
void loop() {
  Serial.println("Pump ON");
  digitalWrite(pumpPin, HIGH);  ///< turn pump ON
  delay(3000);                  ///< wait 3 seconds 

  Serial.println("Pump OFF");   
  digitalWrite(pumpPin, LOW);   ///< turn pump OFF
  delay(3000);                  ///< wait 3 seconds
}
