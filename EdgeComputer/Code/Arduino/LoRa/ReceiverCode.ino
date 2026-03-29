#include <SPI.h>
#include <LoRa.h>

#define LORA_SS       10  // LoRa module CS pin
#define LORA_RESET    9   // LoRa module RESET pin
#define LORA_DIO0     2   // LoRa module DIO0 pin
#define LORA_BAND     915E6  // Frequency for USA (915 MHz)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize LoRa module
  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Receiver Initialized");
}

void loop() {
  // Check if there's a message available
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: ");

    // Read packet
    while (LoRa.available()) {
      String received = LoRa.readString();
      Serial.print(received);
    }
    Serial.println();
  }
}
