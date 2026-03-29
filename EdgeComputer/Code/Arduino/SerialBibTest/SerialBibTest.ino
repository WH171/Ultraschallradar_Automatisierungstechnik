/**
 * @file BluetoothSetup.ino
 * @brief Initialisierung und Kommunikation mit einem HC-05 Bluetooth-Modul.
 * 
 * Dieses Programm konfiguriert das Bluetooth-Modul ueber AT-Befehle
 * und ermoeglicht eine einfache serielle Kommunikation mit einem Hostgeraet.
 */

#include <SoftwareSerial.h>

/// RX-Pin des Bluetooth-Moduls
#define RxD 2

/// TX-Pin des Bluetooth-Moduls
#define TxD 3

/// SoftwareSerial-Objekt zur Bluetooth-Kommunikation
SoftwareSerial blueToothSerial(RxD, TxD);

/**
 * @brief Setup-Funktion zur Initialisierung der seriellen Schnittstellen
 * und Konfiguration des Bluetooth-Moduls.
 */
void setup() {
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
}

/**
 * @brief Hauptschleife zur Verarbeitung eingehender Daten ueber Bluetooth.
 */
void loop() {
  if (blueToothSerial.read() == 'a') {
    blueToothSerial.println("You are connected to Bluetooth Bee");
    // Hier kann weitere Bluetooth-Kommunikation erfolgen
  }
}

/**
 * @brief Fuehrt die Bluetooth-Konfiguration ueber AT-Befehle durch.
 * 
 * Diese Funktion sendet die notwendigen AT-Befehle zur Konfiguration des Moduls
 * wie z.B. Rollenvergabe, Geraetename, Auto-Connect-Optionen und PIN.
 */
void setupBlueToothConnection() {
  Serial.print("Setting up Bluetooth link\n");
  
  blueToothSerial.begin(38400);  // Standard-AT-Baudrate des HC-05
  delay(1000);

  sendBlueToothCommand("\r\n+STWMOD=0\r\n");   // Arbeitsmodus: Slave
  sendBlueToothCommand("\r\n+STNA=modem\r\n"); // Geraetename
  sendBlueToothCommand("\r\n+STAUTO=0\r\n");   // Nicht automatisch verbinden
  sendBlueToothCommand("\r\n+STOAUT=1\r\n");   // Automatischer Bind-Modus
  sendBlueToothCommand("\r\n+STPIN=0000\r\n"); // PIN setzen

  delay(2000);  // Wartezeit nach Konfiguration
  blueToothSerial.print("\r\n+INQ=1\r\n"); // Starte Geraete-Suche
  delay(2000);

  Serial.println("Setup complete");
}

/**
 * @brief Sendet einen AT-Befehl an das Bluetooth-Modul und liest die Antwort aus.
 * 
 * @param command Zeichenkette mit dem AT-Befehl.
 */
void sendBlueToothCommand(char command[]) {
  blueToothSerial.print(command);
  Serial.print(command);
  delay(3000);

  while (blueToothSerial.available()) {
    char a = blueToothSerial.read();
    Serial.print(a);
  }
}
