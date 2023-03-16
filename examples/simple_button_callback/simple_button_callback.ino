/*
 * simple_button_callback.ino
 * 
 * Example program for the mdSimpleButton library
 * <https://github.com/sigmdel/mdSimpleButton>
 *
 */

// SPDX-License-Identifier: 0BSD 

#include <Arduino.h>           // for PlatformIO
#include "mdSimpleButton.h"


#define SERIAL_BAUD 9600
#define BUTTON_PIN 3  
#define ACTIVE LOW

// ---------------------------------------------------


#if (ACTIVE == HIGH) 
  // Connection of an active HIGH normally open button would be
  //                         _________________
  //           __||__       |
  // Vcc ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //         ____      |   
  // GND ---[____]-----+  External pull-down resistor (optional in some cases)
  //              
  // If there's no external pull-down resistor, set useInternalPullResistor to true
  // Many micro-controllers do not have internal pull-down resistors 
  //
  mdSimpleButton button = mdSimpleButton(BUTTON_PIN, false, true);
#else  
  // Connection of an active LOW normally open button would be
  //                         ________________
  //           __||__       |
  // GND ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //         ____      |   
  // Vcc ---[____]-----+  External pull-up resistor (optional in most cases)
  //              
  // If there's no external pull-up resistor, set useInternalPullResistor to true
  // Most micro-controlers have internal pull-up resistors
  // 
  // Active LOW normally open push button without an external pull-up resistor
  // is the default connection
  //
  mdSimpleButton button = mdSimpleButton(BUTTON_PIN);
#endif

bool countsChanged = false;
int pressedCount = 0;
int releasedCount = 0;

void ButtonPressed(buttonEvent event) {
  switch (event) {
     case BUTTON_PRESSED:  
      pressedCount++;
      countsChanged = true;
      Serial.println("Button pressed"); 
      break;
     
    case BUTTON_RELEASED:
      releasedCount++;
      countsChanged = true;
      Serial.println("Button released"); 
      break;

   default: 
      Serial.println("*** SOMETHING HORRIBLY WRONG HAS HAPPENED ***"); 
      break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(2000);
  #if defined(ESP8266)
    Serial.println("\n"); // skip boot garbage
  #endif   

  Serial.println("\nsetup() starting...");

  button.onButtonEvent(ButtonPressed);

  Serial.println("setup() completed.");
}

void loop() {
  button.update();
  delay(40 + random(21)); // rest of loop takes 40 to 60 ms to execute 
  if (countsChanged) {
    countsChanged = false;
    Serial.print("pressedCount = ");
    Serial.println(pressedCount);
    Serial.print("releasedCount = ");
    Serial.println(releasedCount);
  }
}
