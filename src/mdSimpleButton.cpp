/*
 * mdSimpleButton.cpp
 * See mdSimpleButton.h for description, license, etc.
 */

#include <Arduino.h>
#include "mdSimpleButton.h"

#define VERSION 0x000100  // 0.1.0

mdSimpleButton::mdSimpleButton(uint8_t pin, bool activeLow, bool useInternalPullResistor, buttonCallback callback) {
  Serial.println("Constructor");
  _pin = pin;
  _restState = activeLow;
  _active = false;
  _onEvent = callback;

  int mode = INPUT;
  if (useInternalPullResistor) {
    if (activeLow)
      mode = INPUT_PULLUP;
    else {
      // mode = INPUT_PULLDOWN where defined
      #if defined(INPUT_PULLDOWN)
        mode = INPUT_PULLDOWN;
      #elif defined(ESP8266)
        if (pin == 16) mode = INPUT_PULLDOWN_16;
      #endif
    }
  }
  pinMode(_pin, mode);
  version = VERSION;
}

void mdSimpleButton::onButtonEvent(buttonCallback cb) {
  _onEvent = cb;
}

buttonEvent mdSimpleButton::update(void) {
  if ((digitalRead(_pin) != _restState) && (!_active)) {
    _active = true;
    if (_onEvent) _onEvent(BUTTON_PRESSED);
    return BUTTON_PRESSED;
  }
  if ((digitalRead(_pin) == _restState) && (_active)) {
    _active = false;
    if (_onEvent) _onEvent(BUTTON_RELEASED);
    return BUTTON_RELEASED;
  }
  return BUTTON_UNCHANGED;
}
