/**
*
*  @file LEDRGB.h
*
*  @brief header file of the library for using the RGB-LED
*	
*/

#ifndef LEDRGB_h
#define LEDRGB_h



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
int LEDRGBsetup();

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
extern int LEDRGBRed(bool On);


/** 
*  @brief function for switching on or off the green part of the RGB-LED
*  
*  
*  swichting the red green on / off 
*
*  @param On - true: switch on, false: switch off
*
*  @return 0 - success
*  @return 1 - error
*/  
extern int LEDRGBGreen(bool On);

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
extern int LEDRGBBlue(bool On);



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
extern void LEDRGBColor(int r, int g, int b);



#endif