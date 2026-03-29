/**
 * @file anwendungBLE.ino
 * @brief BLE application: send light wavelength to nRF Connect using Arduino Nano 33 BLE Sense.
 * 
 * This sketch reads analog light intensity, converts it to an estimated wavelength,
 * and sends it periodically via BLE to a connected smartphone.
 */

#include <ArduinoBLE.h>  // Include BLE library

/// Name of the BLE device
#define DEVICE_NAME "Nano33_LightSensor"

/// Analog pin for the light sensor
#define LIGHT_SENSOR_PIN A0

/// BLE Service for light sensing
BLEService lightService("180A");  // Custom service UUID

/// BLE characteristic for sending wavelength in nanometers
BLEUnsignedIntCharacteristic wavelengthChar("2A6E", BLERead | BLENotify);

/**
 * @brief Arduino setup function.
 * Initializes BLE, sensor pin and starts advertising.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LIGHT_SENSOR_PIN, INPUT);

  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);
  }

  // BLE device configuration
  BLE.setLocalName(DEVICE_NAME);
  BLE.setAdvertisedService(lightService);

  lightService.addCharacteristic(wavelengthChar);
  BLE.addService(lightService);

  wavelengthChar.writeValue(0);  // Initial value

  BLE.advertise();
  Serial.println("BLE Light Sensor started, advertising...");
}

/**
 * @brief Converts analog value to approximate light wavelength.
 * This is a mock formula for demonstration.
 * @param analogValue ADC value (0-1023)
 * @return Wavelength in nanometers (380-780 nm)
 */
unsigned int convertToWavelength(int analogValue) {
  // Map analog value to 380-780 nm range (visible light)
  return map(analogValue, 0, 1023, 780, 380);
}

/**
 * @brief Arduino loop function.
 * Sends updated wavelength every second if connected.
 */
void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      int sensorValue = analogRead(LIGHT_SENSOR_PIN);
      unsigned int wavelength = convertToWavelength(sensorValue);

      wavelengthChar.writeValue(wavelength);
      Serial.print("Wavelength: ");
      Serial.print(wavelength);
      Serial.println(" nm");

      delay(1000);  // Send data every second
    }

    Serial.println("Disconnected.");
  }
}

