/**
* @file TestHCSR04.ino
* @brief Test program for an ultrasonic sensor
* @details
* Rundenzaehler fuer die Carrera-Bahn
* This program measures the travel time of an ultrasonic signal, calculates the distance using the speed of sound,
* and displays the result in centimeters on the serial monitor.
* Project: Distance Sensor
* @author Gruppe 2
* @date 2025-06
**/

/** Echo pin of the ultrasonic sensor (using pin D6) */
#define Echopin D6
/** Trigger pin of the ultrasonic sensor (using pin D9) */
#define Triggerpin D9

/** Maximum range of the sensor in centimeters */
int MaxReichweite = 215;
/** Minimum range of the sensor in centimeters */
int MinReichweite = 3;
/** Measured distance in centimeters */
double Abstand;
/** Travel time of the ultrasonic signal in microseconds */
double Dauer;
/** Speed of sound in air at 20 \si{\degreeCelsius} in m/s */
double Schallgeschwindigkeit = 343.2;

/**
* @brief Initialization
* @details Starts serial communication and sets the pin modes for trigger and echo.
*/
void setup() {
  Serial.begin(9600);
  pinMode(Triggerpin, OUTPUT);
  pinMode(Echopin, INPUT);
}

/**
* @brief Distance measurement and output
* @details Sends an ultrasonic signal, measures the return time, calculates the distance, and prints it to the serial monitor.
*/
void loop() {
  digitalWrite(Triggerpin, LOW); // Set trigger pin LOW briefly
  delayMicroseconds(2); // Short wait
  digitalWrite(Triggerpin, HIGH); // Send ultrasonic pulse
  delayMicroseconds(10); // Wait to complete pulse
  digitalWrite(Triggerpin, LOW); // Set trigger pin LOW again
  Dauer = pulseIn(Echopin, HIGH); // Measure the time until the echo is received
  Abstand = 0.5 * Schallgeschwindigkeit * Dauer * pow(10,-6) * 100; // Calculate distance in cm
  Serial.print(Abstand);
  Serial.println(" cm");
  delay(5000);
}

