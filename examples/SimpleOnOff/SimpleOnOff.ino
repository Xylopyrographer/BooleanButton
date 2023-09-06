// Boolean Button Library
// https://github.com/Xylopyrographer/BooleanButton
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch to turn an LED on and off with a momentary contact
// push button switch by monitoring the state of a boolean variable.

#include <BooleanButton.h>      // https://github.com/Xylopyrographer/BooleanButton

// pin assignments
//  - change the pin numbers to match your wiring
#define BUTTON_PIN  7       // connect a momentary contact push button switch from this pin to ground
#define LED_PIN     13      // connect an LED with a serial resistor to this pin

bool myBoolean = 0;         // the boolean variable we are monitoring the state of

// define the BoolBtn object (note: we need to pass the address (a pointer)
// to the variable, so need to prefix the variable name with '&')
BoolBtn myVirtualBtn( &myBoolean, 25, true );   // 'invert' is set to true as we're using a button wired to ground

void setup() {
    myVirtualBtn.begin();                   // initialize the boolean button object
    pinMode( BUTTON_PIN, INPUT_PULLUP );    // set the button pin as an input
    pinMode( LED_PIN, OUTPUT );             // set the LED pin as an output
    digitalWrite( LED_PIN, LOW );           // make sure the LED is off

}

void loop() {
    // get the current state of the button into the boolean variable being monitored
    myBoolean = digitalRead( BUTTON_PIN );    
    myVirtualBtn.read();                    // read the variable state

    static bool ledState;                   // a variable that keeps the current LED status

    if ( myVirtualBtn.wasReleased() ) {     // if the button was released, toggle the LED state
        ledState = !ledState;
        digitalWrite( LED_PIN, ledState );
    }

}

