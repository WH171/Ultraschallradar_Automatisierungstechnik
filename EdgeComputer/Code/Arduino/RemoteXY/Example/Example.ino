/**
 * @file Rundenzaehler.ino
 * @brief Slot car lap counter system with mobile app interface
 * @details
 * Rundenzaehler fuer die Carrera-Bahn
 * This implementation of a lap timing system uses the RemoteXY library for mobile GUI control. 
 * The system tracks lap times for two vehicles using HC-SR04 ultrasonic sensors and transmits 
 * data via Bluetooth (HC-05) to a smartphone app. Two race modes are supported: Sprint (3 laps) 
 * and Grand Prix (10 laps). The software continuously records lap times, calculates best laps 
 * per vehicle, and displays results in real-time. Hardware connections include ultrasonic 
 * sensors on pins D6/D9 (Car 1) and D7/D10 (Car 2), plus status LEDs for system feedback. 
 * Proper operation requires individual sensor calibration for the specific track configuration.
 * @author Gruppe 2
 * @date 2025-06
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
uint8_t RemoteXY_CONF[] =   // 317 bytes
  { 255,3,0,24,0,54,1,19,0,0,0,82,117,110,100,101,110,122,195,164,
  104,108,101,114,0,31,1,106,200,1,1,24,0,12,14,11,82,10,192,30,
  26,83,112,114,105,110,116,114,101,110,110,101,110,0,71,114,97,110,100,112,
  114,105,120,0,1,68,165,17,17,0,133,31,0,67,52,29,40,10,78,2,
  26,2,129,10,31,34,9,64,24,76,105,118,101,32,90,101,105,116,0,129,
  35,3,41,6,64,24,77,111,100,117,115,97,117,115,119,97,104,108,0,129,
  5,51,16,10,64,8,67,97,114,0,129,8,85,7,12,64,8,50,0,129,
  69,52,23,8,64,8,82,117,110,100,101,0,130,62,66,33,15,27,17,67,
  64,68,29,11,78,2,26,2,129,60,151,36,12,64,24,83,84,65,82,84,
  0,129,9,107,89,9,64,24,83,99,104,110,101,108,108,115,116,101,32,82,
  117,110,100,101,110,122,101,105,116,0,130,30,121,45,16,27,17,67,34,123,
  37,12,78,2,26,2,129,7,68,7,12,64,8,49,0,130,62,83,33,15,
  27,17,67,64,85,29,11,78,2,26,2,130,25,66,33,15,27,17,67,27,
  68,29,11,78,2,26,2,130,25,83,33,15,27,17,67,27,85,29,11,78,
  2,26,2,129,37,52,14,8,64,8,90,101,105,116,0,1,17,165,17,17,
  0,37,31,0,129,9,151,32,12,64,24,83,116,111,112,112,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t Modusauswahl; // from 0 to 2
  uint8_t StartButton; // =1 if button pressed, else =0
  uint8_t StoppButton; // =1 if button pressed, else =0

    // output variables
  float LiveZeit;
  float RundeAuto1;
  float SchellsteRundeGesamt;
  float RundeAuto2;
  float RundenzeitAuto1;
  float RundenzeitAuto2;

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}