#include <LEDGeneral.h>
#include <LEDPower.h>
#include <LEDRGB.h>
#include <LEDSignsOfLife.h>
#include <LEDbuiltin.h>

/**
*
*  @file TestPushButtonInterrupt.ino
*
*  @brief Simple application reading in the built-in push button states using an interrupt
*	
*
*  @details If the builtin push button is pressed, the built-in LED is switched on  for 2 second and switched off again.
*  But in this example, an interrupt is used.
*  
*/

#include <LEDGeneral.h>
#include <LEDBuiltin.h>



#define BUTTON_B 13 /*<   Button_B is here defined as pin 11 */ 
#define BUTTON_PIN  BUTTON_B


#define SET_ON  true  /*< Define flag for switching on  */
#define SET_OFF false /*< Define flag for switching off */


bool buttonState = false; /*< state of the built-in push button */

 
// Initialize variables
//
volatile bool pushPressed = false; /*< // Flag, whether the button is pressed. Declare as volatile for interrupt.  safety */

int ledState = 0; /*<  LED-Status zur Verarbeitng */

// Variables for button debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool lastButtonState = HIGH;


// Custom shield button pin setting
void nrf_gpio_cfg_out_with_input(uint32_t pin_number) {
  nrf_gpio_cfg(
    pin_number,
    NRF_GPIO_PIN_DIR_OUTPUT,
    NRF_GPIO_PIN_INPUT_CONNECT,
    NRF_GPIO_PIN_PULLUP,
    NRF_GPIO_PIN_S0S1,
    NRF_GPIO_PIN_NOSENSE);
}

// set the pin modes for the button
void initializeShield(){
  pinMode(BUTTON_PIN, OUTPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  nrf_gpio_cfg_out_with_input(digitalPinToPinName(BUTTON_PIN));
}

// debounce the button and return true on new click
bool readShieldButton(){
  bool buttonRead = nrf_gpio_pin_read(digitalPinToPinName(BUTTON_PIN));
  
  if (buttonRead != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    if (buttonRead != buttonState) {
      buttonState = buttonRead;

      if (!buttonState) {
        lastButtonState = buttonRead;
        return true;
      }
    }
  }

  lastButtonState = buttonRead;
  return false;
}

/**  
*  @brief the setup function runs once when you press reset or power the board
*
*  standard function of Arduino sketches
*  
*  Initialization of the built-inb LED and the push button
*
*  @param ---
*
*  @return void
*/  
void setup() {
  // Initialize the state
  pushPressed = false;
  // Initialize the pin as an output
  LEDBuiltinsetup();
  // Initialize the pin as an input
  pinMode(BUTTON_PIN, INPUT);

  initializeShield();
  // Initialize the interrupt function    
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, CHANGE);

  Serial.begin(9600);

}
        

/**  
*  @brief Interrupt service function
*
*  Attention: as short as possible
*
*  The function changes just one flag.
*/
void buttonPressed() 
{
  if (pushPressed == false) 
  {
    pushPressed = true;
  }	
}

/** 
*  @brief the loop function runs over and over again forever
*  
*  standard function of Arduino sketches
*  
*  swichting the built-in led on for 2 sec, if the push button is pressed
*
*  @param ---
*
*  @return void
*/  
void loop() 
{
  if (pushPressed)
  {
    Serial.println("Interrupt");
    // Turn the LED on
    LEDBuiltin(SET_ON);
    // Wait for two second
    delay(2000);
    // Turn the LED off
    LEDBuiltin(SET_OFF);
    pushPressed = false;	  
  }	


  LEDBuiltin(SET_ON);
  // Wait for two second
  delay(500);
  // Turn the LED off
  LEDBuiltin(SET_OFF);
  delay(500);
 
   if (readShieldButton()) 
   {
     LEDBuiltin(SET_ON);
    // Wait for two second
    delay(3000);
    // Turn the LED off
    LEDBuiltin(SET_OFF);
 

   }
 
  // ... 
}