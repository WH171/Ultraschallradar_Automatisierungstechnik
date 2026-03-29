/*
   -- RemoteXYBib --
   
   Dieser Quellcode der grafischen Benutzeroberflaeche 
   wurde automatisch vom RemoteXY-Editor erzeugt.
   Um diesen Code mit der RemoteXY-Bibliothek 3.1.13 oder einer neueren Version zu kompilieren, 
   lade die Bibliothek unter folgendem Link herunter: http://remotexy.com/en/library/
   Um eine Verbindung mit der RemoteXY-Mobile-App herzustellen, lade sie hier herunter: http://remotexy.com/en/download/                   
     - fuer ANDROID 4.15.01 oder eine neuere Version;
    
   Dieser Quellcode ist freie Software; du kannst ihn unter den Bedingungen der GNU Lesser General Public
   License weiterverbreiten und/oder aendern, wie von der Free Software Foundation veroeffentlicht;
   entweder in der Version 2.1 der Lizenz oder (nach deiner Wahl) in einer spaeteren Version.    
*/

//////////////////////////////////////////////
//        RemoteXY Include-Bibliothek       //
//////////////////////////////////////////////

// Du kannst das Debug-Logging zur seriellen Ausgabe mit 115200 Baud aktivieren
//#define REMOTEXY__DEBUGLOG    

// RemoteXY Verbindungsmodus und Bibliothek einbinden
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY Verbindungs-Einstellungen
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600


#include <RemoteXY.h>

// RemoteXY GUI-Konfiguration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 29 Bytes
  { 255,1,0,0,0,22,0,19,0,0,0,0,31,1,106,200,1,1,1,0,
  1,37,59,24,24,0,2,31,0 };
  
// Diese Struktur definiert alle Variablen und Ereignisse deiner Steueroberflaeche 
struct {

    // Eingabe-Variablen
  uint8_t button_01; // =1, wenn Knopf gedrueckt, sonst =0

    // Andere Variablen
  uint8_t connect_flag;  // =1, wenn drahtlos verbunden, sonst =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           ENDE RemoteXY Include         //
/////////////////////////////////////////////



/**
 * @brief Initialisiert die RemoteXY-Verbindung und setzt notwendige Einstellungen.
 * 
 * Diese Funktion wird einmalig beim Start des Programms aufgerufen.
 * Sie initialisiert die RemoteXY-Bibliothek und stellt die Verbindung zum
 * Steuerungssystem her.
 */
void setup() 
{
  RemoteXY_Init (); 
  
  // TODO: Setup-Code hinzufuegen
  
}

/**
 * @brief Haupt-Loop der Steuerung.
 * 
 * Diese Funktion wird kontinuierlich in der Loop ausgefuehrt. Sie aktualisiert
 * die Eingaben von der RemoteXY-App und ermoeglicht die Kommunikation mit
 * der Steueroberflaeche. Verwende die RemoteXY-Struktur fuer den Datentransfer.
 * Verwende anstelle von delay() die RemoteXY_delay() Funktion.
 */
void loop() 
{ 
  RemoteXY_Handler ();
  
  // TODO: Loop-Code hinzufuegen
  // Verwende die RemoteXY-Struktur fuer den Datentransfer
  // Vermeide die Verwendung von delay(), verwende stattdessen RemoteXY_delay()
}
