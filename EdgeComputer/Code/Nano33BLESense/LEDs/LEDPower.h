/**
*
*  @file LEDPower.h
*
*  @brief header file of the library for using the power LED
*	
*/

#ifndef LEDPower_h
#define LEDPower_h



/**  
*  @brief initialize digital pin LED_PWR as an output.
*
*  call the function once when you press reset or power the board
*
*  Initialization of the pin LED_PWR as output
*
*  @param ---
*
*  @return 0 - success
*  @return 1 - error
*/  
extern int LEDPowersetup();

/** 
*  @brief function for switching on or off the LED
*  
*  
*  swichting the led on / off 
*
*  @param On - true: switch on, false: switch off
*
*  @return 0 - success
*  @return 1 - error
*/  
extern int LEDPower(bool On);

#endif