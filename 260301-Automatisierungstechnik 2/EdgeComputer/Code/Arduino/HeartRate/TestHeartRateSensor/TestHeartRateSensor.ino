// Code to test the GRV Heart Rate 3 sensor.
// The sensor's raw values are output to the serial monitor at an interval of 500 ms.

// file: TestHeartRate.ino

int heartRatePin = A0;  
int heartRateValue = 0;  

// Timing variables for controlling reading intervals
unsigned long previousMillis = 0;  
const long interval = 500;  // Interval for readings in milliseconds

void setup() {
  Serial.begin(9600);  
  pinMode(heartRatePin, INPUT);  
}

void loop() {
  // Get the current time in milliseconds
  unsigned long currentMillis = millis();

  // Check if the specified interval has passed
  if (currentMillis - previousMillis >= interval) {
    // Update the previous time marker
    previousMillis = currentMillis;

    // Read the analog value from the heart rate sensor
    heartRateValue = analogRead(heartRatePin);
    // Output the heart rate value to the serial monitor
    Serial.println(heartRateValue);  
  }
}
