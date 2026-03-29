/**
*
*  @file LEDSignsOfLife.h
*
*  @brief header file for the function signs of life with a LED
*	
*
*  
*/

#ifndef LEDSignsOfLife_h
#define LEDSignsOfLife_h



/**  
*  @brief initialize digital pin of the LED as an output.
*
*  call the function once, in the function setup, when you press reset or power the board
*
*  Initialization of the pin  as an output
*
*  The program doesn't check if the parameter PinNo is reasonable.
*
*  @param PinNo - number of pin for the LED
*  @param CycleTimeOn - duration of the LED is on
*  @param CycleTimeOff - duration of the LED is off
*
*  @return 0 - success
*  @return 1 - error
*/  
extern bool SignsOfLifeSetup(int PinNO, int CycleTimeOn, int CycleTimeOff);

/** 
*  @brief function for blinking the LED
*  
*  
*  The function checks the duration of the state. If the state has to change, then the function changes the state.
*   

*
*  @return true - LED's actual state is on
*  @return false - LED's actual state is off
*/  
extern bool SignsOfLife();




#endif