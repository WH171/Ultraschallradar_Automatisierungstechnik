/**
 * @file TestBLEBlink.ino
 * @brief RemoteXY BLE Example - Control onboard LED via Bluetooth
 * 
 * This example demonstrates how to control the onboard LED of an Arduino Nano 33 BLE 
 * using the RemoteXY app over a Bluetooth Low Energy (BLE) connection.
 * 
 * The interface contains a single button. When pressed, the onboard LED is turned ON; 
 * when released, the LED is turned OFF.
 * 
 * @details
 * - Connection type: BLE (ArduinoBLE)
 * - RemoteXY Library required: version 3.1.13 or later
 * - Compatible with Android (v4.15.01+) and iOS (v1.12.1+)
 * 
 * @see https://remotexy.com/en/library/
 * @see https://remotexy.com/en/examples/
 * 
 * @version 1.0
 * @date 2025-05-19
 * @author Moslen
 * 
 * @license GNU Lesser General Public License, Version 2.1 or later
 */

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// Enable debug logging to Serial at 115200 baud if needed
//#define REMOTEXY__DEBUGLOG    

// RemoteXY connection mode selection and library include
#define REMOTEXY_MODE__ARDUINOBLE

#include <ArduinoBLE.h>

// Bluetooth device name for advertising
#define REMOTEXY_BLUETOOTH_NAME "TestBLEBlink"

#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)
/**
 * @brief RemoteXY configuration array for GUI layout
 * 
 * This array defines the layout and properties of the user interface
 * generated with the RemoteXY editor.
 */
uint8_t RemoteXY_CONF[] =   // 29 bytes
  { 255,1,0,0,0,22,0,19,0,0,0,0,31,1,106,200,1,1,1,0,
  1,36,77,24,24,0,2,31,0 };

/**
 * @struct RemoteXY
 * @brief Structure holding all input and status variables from the GUI
 */
struct {

    /** 
     * @brief Button state
     * @details =1 if button is pressed, =0 otherwise
     */
    uint8_t button_01;

    /** 
     * @brief Connection status flag
     * @details =1 if BLE is connected, =0 if disconnected
     */
    uint8_t connect_flag;

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

// Define LED pin (onboard LED for Nano 33 BLE)
#define PIN_LED LED_BUILTIN

/**
 * @brief Arduino setup function
 * 
 * Initializes RemoteXY and sets the onboard LED pin as output.
 */
void setup()  
{ 
  RemoteXY_Init();  
  pinMode(PIN_LED, OUTPUT);
} 

/**
 * @brief Arduino loop function
 * 
 * Handles RemoteXY events and toggles the LED based on button input.
 */
void loop()  
{  
  RemoteXY_Handler();

  // Turn LED on/off based on the RemoteXY button state
  digitalWrite(PIN_LED, (RemoteXY.button_01 == 1) ? HIGH : LOW); 
}

