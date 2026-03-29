/**
*
*  @file LEDRGB.cpp
*
*  @brief file of the library for using the RGB-LED
*	
*
*  
*/


#include <LEDGeneral.h>
#include <LEDRGB.h>
#include <Arduino.h>


#define LED_RED   22 /*< Define the pin for the red RGB-LED */
#define LED_GREEN 23 /*< Define the pin for the green RGB-LED */
#define LED_BLUE  24 /*< Define the pin for the blue RGB-LED */

/**  
*  @brief initialize the digital pins LED_RED, LED_GREEN, and LED_BLUE as output.
*
*  call the function once when you press reset or power the board
*
*  Initialization of the pins LED_RED, LED_GREEN, and LED_BLUE as output
*
*  @param ---
*
*  @return 0 - success
*  @return 1 - error
*/  
int LEDRGBsetup()
{
  // initialize digital pin LED_BUILTIN as an output.
  LEDSetup(LED_RED);
  LEDSetup(LED_GREEN);
  LEDSetup(LED_BLUE);
  
  return 0;
}

/** 
*  @brief function for switching on or off the red part of the RGB-LED
*  
*  
*  swichting the red led on / off 
*
*  @param On - true: switch on, false: switch off
*
*  @return 0 - success
*  @return 1 - error
*/  
int LEDRGBRed(bool On)
{
  LED(LED_RED, On);   // turn the LED on = true or off = false
  
  return 0;
}

/** 
*  @brief function for switching on or off the green part of the RGB-LED
*  
*  
*  swichting the green led on / off 
*
*  @param On - true: switch on, false: switch off
*
*  @return 0 - success
*  @return 1 - error
*/  
int LEDRGBGreen(bool On)
{
  LED(LED_GREEN, On);   // turn the LED on = true or off = false
  
  return 0;
}

/** 
*  @brief function for switching on or off the blue part of the RGB-LED
*  
*  
*  swichting the blue led on / off 
*
*  @param On - true: switch on, false: switch off
*
*  @return 0 - success
*  @return 1 - error
*/  
int LEDRGBBlue(bool On)
{
  LED(LED_BLUE, On);   // turn the LED on = true or off = false
  
  return 0;
}

/** 
*  @brief function for setting a rgb color for the RGB-LED
*  
*  The functions sets the rgb color of the built-in RGB-LED.
*  
*
*  @param r - integer [0, 255]: red part
*  @param g - integer [0, 255]: green part
*  @param b - integer [0, 255]: blue part
*/  
void LEDRGBColor(int r, int g, int b)
{
    // Write the PWM values to the LED pins
    analogWrite(LED_RED, r);
    analogWrite(LED_GREEN, g);
    analogWrite(LED_BLUE, b);
}
