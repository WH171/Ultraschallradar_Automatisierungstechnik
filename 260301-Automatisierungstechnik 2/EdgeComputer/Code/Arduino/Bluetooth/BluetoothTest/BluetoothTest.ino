/*
   -- BluetoothTest --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__HARDSERIAL


// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial1
#define REMOTEXY_SERIAL_SPEED 9600


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 47 bytes
  { 255,3,0,0,0,40,0,19,0,0,0,0,31,2,106,200,200,84,1,1,
  2,0,4,46,111,14,36,131,19,15,46,0,2,26,5,20,27,60,60,31,
  15,53,53,0,2,26,31 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t slider_01; // from 0 to 100
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
//////////////////////////////////////////////////
//                 Setup-Funktion               //
//////////////////////////////////////////////////

/**
 * @brief Initialisiert die RemoteXY-Schnittstelle und serielle Kommunikation.
 */
void setup() {
  RemoteXY_Init();         ///< Initialisiert die RemoteXY-Verbindung
  Serial.begin(9600);      ///< Startet serielle Kommunikation mit 9600 Baud
}

//////////////////////////////////////////////////
//                 Loop-Funktion                //
//////////////////////////////////////////////////

/**
 * @brief Hauptprogrammschleife: verarbeitet GUI-Eingaben und gibt Werte aus.
 */
void loop() {
  RemoteXY_Handler();      ///< Verarbeitet Benutzereingaben aus der RemoteXY-App

  Serial.println("Sliderposition:");
  Serial.print(RemoteXY.slider_01);        ///< Gibt den Wert des Schiebereglers aus

  Serial.println("Joystick X:");
  Serial.print(RemoteXY.joystick_01_x);    ///< Gibt den X-Wert des Joysticks aus

  Serial.println("Joystick Y:");
  Serial.print(RemoteXY.joystick_01_y);    ///< Gibt den Y-Wert des Joysticks aus

  RemoteXY_delay(200);     ///< Wartet 200 Millisekunden (empfohlene Methode)
}
