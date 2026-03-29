/**
 * @file rfid_unlock.ino
 * @brief This sketch demonstrates the use of an MFRC522 RFID reader with an Arduino.
 *
 * It initializes the RFID module, reads the UID of a presented RFID card,
 * compares it against a stored UID, and prints whether the card is known or unknown.
 */

#include <SPI.h>          ///< Include the SPI library for communication with the MFRC522 module.
#include <MFRC522.h>      ///< Include the MFRC522 library for controlling the RFID reader module.

#define SS_PIN 10         ///< Slave Select (SS) pin for the RFID module.
#define RST_PIN 9         ///< Reset (RST) pin for the RFID module.

// Create an MFRC522 object with the defined SS and RST pins.
MFRC522 rfid(SS_PIN, RST_PIN); 

// Define an instance of a MIFARE key (not used for authentication in this example).
MFRC522::MIFARE_Key key;

// Define a UID that will be considered as the valid unlock card.
// Example UID: {32, 154, 149, 117}
int code[] = {32, 154, 149, 117}; 

int codeRead = 0;         ///< Variable not used in this version of the sketch.
String uidString;         ///< String representation of the read UID.

/**
 * @brief Arduino setup function.
 *
 * Initializes serial communication, SPI interface, and RFID reader.
 */
void setup() {
    Serial.begin(9600);              // Initialize serial communication.
    while (!Serial);                 // Wait until the Serial is ready (especially needed for Leonardo/Micro).
    Serial.println("Hallo Welt");    // Initial message in German.

    SPI.begin();                     // Initialize SPI bus.
    rfid.PCD_Init();                 // Initialize the MFRC522 RFID reader.

    Serial.println(F("Arduino RFID")); // Print startup message.
}

/**
 * @brief Arduino loop function.
 *
 * Continuously checks if a new RFID card is present and calls readRFID() if it is.
 */
void loop() {
    // Check if a new card is present.
    if (rfid.PICC_IsNewCardPresent()) {
        readRFID();  // Handle reading and checking the card.
    }
    delay(100);  // Short delay to prevent spamming.
}

/**
 * @brief Reads an RFID card and checks its UID against a predefined value.
 *
 * If the UID matches the stored code, it prints "*** Unlock ***".
 * Otherwise, it prints "Unknown Card".
 * After processing, the card is halted and encryption is stopped.
 */
void readRFID() {
    // Read the UID from the presented RFID card.
    rfid.PICC_ReadCardSerial();

    Serial.print(F("\nPICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

    // Check if the card is of a supported MIFARE Classic type.
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return;
    }

    // Display the UID in human-readable format.
    Serial.println("Scanned PICC's UID:");
    printDec(rfid.uid.uidByte, rfid.uid.size);

    // Convert UID to a string format for logging or debug.
    uidString = String(rfid.uid.uidByte[0]) + " " +
                String(rfid.uid.uidByte[1]) + " " +
                String(rfid.uid.uidByte[2]) + " " +
                String(rfid.uid.uidByte[3]);

    // Compare the read UID with the stored code.
    int i = 0;
    boolean match = true;
    while (i < rfid.uid.size) {
        if (int(rfid.uid.uidByte[i]) != code[i]) {
            match = false;
        }
        i++;
    }

    // Print result of UID check.
    if (match) {
        Serial.println("\n*** Unlock ***");
    } else {
        Serial.println("\nUnknown Card");
    }

    Serial.println("============================");

    // Halt the card communication.
    rfid.PICC_HaltA();

    // Stop encryption on the PCD.
    rfid.PCD_StopCrypto1();
}

/**
 * @brief Prints a byte array in decimal format to the serial monitor.
 *
 * @param buffer Pointer to the byte array.
 * @param bufferSize Number of bytes in the array.
 */
void printDec(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
