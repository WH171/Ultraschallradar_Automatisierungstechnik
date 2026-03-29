// How to control the power LED of the Arduino Nano 33 BLE Sense //
// file : TestLEDPower.ino //
// The LED is switched on for 1 second and switched off // for 1 second so that the LED flashes accordingly .

#define LED_PWR 25 // Define the pin for the power LED
		
void setup () {
	// Initialize the pin as an output
	pinMode(LED_PWR, OUTPUT) ;
}
		
void loop () {
		// Turn the LED on
		digitalWrite (LED_PWR, HIGH) ;
		// Wait for one second
		delay (1000) ;
		// Turn the LED off
		digitalWrite (LED_PWR, LOW) ;
		// Wait for one second
		delay (1000) ;
}