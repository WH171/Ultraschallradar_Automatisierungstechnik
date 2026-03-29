/**
*
*  @file LEDPower.cpp
*
*  @brief file of the library for using the power LED
*	
*
*  
*/


#include <LEDGeneral.h>
#include <LEDPower.h>

#define LED_PWR 25 /*< Define the pin for the power LED */


/**  
*  @brief initialize digital pin LED_BUILTIN as an output.
*
*  call the function once when you press reset or power the board
*
*  Initialization of the pin LED_BUILTIN as output
*
*  @param ---
*
*  @return 0 - success
*  @return 1 - error
*/  
int LEDPowersetup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  LEDSetup(LED_PWR);
  
  return 0;
}

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
int LEDPower(bool On) 
{
  LED(LED_PWR, On);   // turn the LED on = true or off = false
  
  return 0;
}