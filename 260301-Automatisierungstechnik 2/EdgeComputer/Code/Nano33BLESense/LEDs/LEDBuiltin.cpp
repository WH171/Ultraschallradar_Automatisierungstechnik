/**
*
*  @file LEDBuiltin.cpp
*
*  @brief file of the library for using the built-in LED
*	
*
*  
*/


#include <LEDGeneral.h>
#include <LEDBuiltin.h>


#define LED_BUILTINLED 13 /*< Define the pin for the builtin LED */


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
int LEDBuiltinsetup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  LEDSetup(LED_BUILTINLED);
  
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
int LEDBuiltin(bool On) 
{
  LED(LED_BUILTINLED, On);   // turn the LED on = true or off = false
  
  return 0;
}