# mdSimpleButton Library

**Version 0.2.1** (July 20, 2024)

An [Arduino](https://www.arduino.cc/) library that returns changes in the state of an on/off button.

## mdSimpleButton Class Constructor

An instance of the `mdSimpleButton` class must be created for each button. 

```cpp
mdSimpleButton(uint8_t pin, bool activeLow = true, bool useInternalPullResistor = true, buttonCallback cb = nullptr);
```

The GPIO pin connected to one terminal of the button must be specified in the constructor. The other parameters of the constructor have default values that correspond to a normally open push button with the other pin connected to ground:

```cpp
mdSimpleButton myButton mdSimpleButton(3);
```

When the button is mechanically set to the ON position
the signal from the button transitions from a `HIGH` to a `LOW` value.
Consequently, when the button is returned to the OFF position 
the signal from the button transitions from `LOW` to `HIGH`.
This can be reversed by setting `activeLow` to `false` in which case a 
normally open push button would be connected to Vcc. 

If `useInternalPullResistor` is set to `true`, which is the default, 
then an internal pull-up or pull-down resistor will be enabled. If 
`activeLow = true`, then an internal pull-up resistor will be 
used to keep the GPIO pin `HIGH`  when the button is in the OFF position.
If `activeLow = false`, then an internal pull-down resistor will be enabled to keep the GPIO pin `HIGH` when the button is in the OFF position 
assuming that the microcontroller has pull-down resistors. If 
`useInternalPullResistor` is set to `false` neither pull-up nor pull-down
resistors are enabled. It will probably be necessary to have an external pull-up 
or pull-down resistor connected to the GPIO pin used with the button.


## Button Events

The state of the push button is updated when the function 

```cpp
  buttonEvent update(void)
```    
is invoked. This function must be called at regular intervals which
is usually done in the `loop()` function of the sketch. The value 
returned by the function is a button event which indicates any change in 
the state of the button.

```cpp
enum buttonEvent {
  BUTTON_UNCHANGED = 0,   // the button state has not changed 
  BUTTON_PRESSED,         // the button has been put into the ON position
  BUTTON_RELEASED,        // the button has been put into the OFF position
  BUTTON_LONGPRESS        // the button has been put into the OFF position after a long time ON        
};
```

## Debounce and Long Press Timing

Because a button is a mechanical object containing a spring or equivalent mechanism, the contact will initially be broken and made again a number of times immediately after the button is pressed. This is called switch bounce. With microcontrollers becoming very fast, and because some buttons are very noisy, the BUTTON_RELEASE event can be reported before the make event has actually settled down. To avoid this problem, the mdSimpleButton class ignores make and break events during a `debounce` interval after the first time the button is pressed.

Since the class keeps track of the time when the `BUTTON_PRESSED` event occured, it will know the time elapsed until the button is set to OFF. If this time period is equal or greater to `longpress` then a `BUTTON_LONGPRESS` event is returned by `update()` otherwise a `BUTTON_RELEASED` is returned.

The default timing values are set with macros in the `mdSimpleButton.h` header file.

```cpp
#define DEBOUNCE_TIME    50 // ms of debounce time before accepting release event
#define LONGPRESS_TIME 1000 // ms minimum time between press and release to constitute a long button press
```

The default debounce time of 50 ms is relatively aggressive and may need to be adjusted for some buttons. Similarly, the 1 second interval is a completely arbitrary value. Both of these can be set.

```cpp
// press key debounce time (ms)
uint32_t mdSimpleButton::debounce;       // default is DEBOUNCE_TIME

// minimum key depressed time (ms) to qualify as a long key press
uint32_t mdSimpleButton::longpress;      // default is LONGPRESS_TIME
```

## Usage

Here is a very simple example:

```cpp
#include "Arduino.h"
#include "mdSimpleButton.h"

// Normally open push button connected to ground and I/O pin 3, using internal pullup resistor
mdSimpleButton myButton = mdSimpleButton(3);

void setup(void) {
  ... 
  myButton.debounce = 75;   // extra debounce time
  myButton.longpress = 750; // 3/4 second for a long press
  ...
}

void loop() {
  ... 
  switch (myButton.update()) {
    case BUTTON_PRESSED:  
      Serial.println("Button pressed"); 
      break;    
    case BUTTON_RELEASED:
      Serial.println("Button released"
      break;   
    case BUTTON_LONGPRESS:
      Serial.println("Button released after a long press"); 
      break;
  } 
  ... 
}
```

## Callback Functions

Instead of testing the returned value of `update()` to see if the button has been pressed or released, a callback function can be specified. The callback will never be called on a `BUTTON_UNCHANGED` event. 

The callback function can handle more than one button because a pointer to the `mdSimpleButton` instance is passed back as a parameter.

```cpp
  void buttonPressed(mdSimpleButton* button, buttonEvent event) {
    if (button->pin() == 3) {
      // handle button on I/O pin 3
      ... 
    } else if (button->pin() == 8) {
      // handle button on I/O pin 8
      if ( (event == BUTTON_LONGPRESS) && (button->presstime > 5000) ) {
        Serial.println("Really long button press!");        
  ...    
```

It is still necessary to call `update()` in `loop()`. Don't forget that the update of `mdSimpleButton` instances must be called. This callback is not an interrupt service routine. Since the callback is executing within the `loop()` function, there is no need to worry about the reentrancy of functions it calls.

A callback is assigned with the `onButtonEvent()` method in `setup()` or in the button constructor. 

# Examples

Hopefully, the examples illustrate how to use this simple library. 
The first, `simple_button_basic.ino` uses the returned value from the
`update()` function directly and must therefore correctly handle a 
returned `BUTTON_UNCHANGED` which indicates that there has been no change in the state of the button.

The second example, `simple_button_callback.ino` is similar but uses a 
callback function which will never have to handle a `BUTTON_UNCHANGED` event. 
Note that `update()` is still called in the `loop()` function. 

# Architecture and Testing

The library should work with most microcontrollers with an Arduino core. However it has only been tested 
with the ESP32-C3 in Arduino-ESP32. Furthermore it has only been tested with two normally open push buttons connected to an I/O pin and ground. There are many more combinations to look at.

# Credits

Based on [MicroPython-Button](https://github.com/ubidefeo/MicroPython-Button) by Ubi de Feo (ubidefeo)

# Change Log

July 20, 2024 (Version 0.2.1)
  - removed Serial.print() statement in constructor that could interfere with Serial.available().

March 16, 2023 (Version 0.2.0)
  - first public commit

# Licence

The **BSD Zero Clause** ([SPDX](https://spdx.dev/): [0BSD](https://spdx.org/licenses/0BSD.html)) licence applies.
