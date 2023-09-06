// Boolean Button Library
// https://github.com/Xylopyrographer/BooleanButton
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch demonstrating short and long button presses.
//
// A simple state machine where a short press of a button connected
// to a GPIO pin turns an LED connected to a GPIO pin on or off, and
// a long press causes the LED to blink rapidly. Once in rapid blink
// mode, another long press goes back to on/off mode.

#include <BooleanButton.h>      // https://github.com/Xylopyrographer/BooleanButton

// pin assignments
//  - change the pin numbers to match your wiring
#define BUTTON_PIN  14              // connect a button switch from this pin to ground
#define LED_PIN     25              // connect an LED with a serial resistor to this pin

#define LONG_PRESS     1000         // we define a "long press" to be 1000 milliseconds.
#define BLINK_INTERVAL 100          // in the BLINK state, switch the LED every 100 milliseconds.

// the list of possible states for the state machine. This state machine has a fixed
// sequence of states, i.e. ONOFF --> TO_BLINK --> BLINK --> TO_ONOFF --> ONOFF
// note that while the user perceives two "modes", i.e. ON/OFF mode and rapid blink mode,
// two extra states are needed in the state machine to transition between these modes.
enum states_t { ONOFF, TO_BLINK, BLINK, TO_ONOFF };

bool ledState;                  // current LED status
unsigned long ms;               // current time from millis()
unsigned long msLast;           // last time the LED was switched

bool myBoolean = 0;             // the variable to monitor
// define the BoolBtn object (note: we need to pass the address (a pointer)
// to the variable, so need to prefix the variable name with '&')
BoolBtn myVirtualBtn( &myBoolean, 25, true );   // 'invert' is set to true as we're using a button wired to ground

// function to reverse the current LED state.
//  - if it's on, turn it off. if it's off, turn it on.
void switchLED() {
    msLast = ms;                // record the last switch time
    ledState = !ledState;
    digitalWrite( LED_PIN, ledState );
}

// function to switch the LED on and off every BLINK_INTERVAL milliseconds.
void fastBlink() {
    if ( ms - msLast >= BLINK_INTERVAL )
        switchLED();
}

void setup() {
    pinMode( BUTTON_PIN, INPUT_PULLUP );    // set the button pin as an input
    pinMode( LED_PIN, OUTPUT );             // set the LED pin as an output
    digitalWrite( LED_PIN, LOW );           // make sure the LED is off

    myVirtualBtn.begin();                   // initialize the boolean button object
    myBoolean = digitalRead( BUTTON_PIN );  // set the variable we are monitoring to the state of the button
    myVirtualBtn.read();                    // read the variable state
}

void loop() {
    ms = millis();                          // record the current time

    myBoolean = digitalRead( BUTTON_PIN );  // set the variable we are monitoring to the state of the button
    myVirtualBtn.read();                    // read the variable state

    static states_t STATE;                  // current state machine state

    switch ( STATE ) {
        // this state watches for short and long presses, switches the LED for
        // short presses, and moves to the TO_BLINK state for long presses.
        case ONOFF:
            if ( myVirtualBtn.wasReleased() )
                switchLED();
            else if ( myVirtualBtn.pressedFor( LONG_PRESS ) )
                STATE = TO_BLINK;
            break;

        // this is a transition state where we start the fast blink as feedback to the user,
        // but we also need to wait for the user to release the button, i.e. end the
        // long press, before moving to the BLINK state.
        case TO_BLINK:
            if ( myVirtualBtn.wasReleased() )
                STATE = BLINK;
            else
                fastBlink();
            break;

        // the fast-blink state. Watch for another long press which will cause us to
        // turn the LED off (as feedback to the user) and move to the TO_ONOFF state.
        case BLINK:
            if ( myVirtualBtn.pressedFor( LONG_PRESS ) ) {
                STATE = TO_ONOFF;
                digitalWrite( LED_PIN, LOW );
                ledState = false;
            }
            else
                fastBlink();
            break;

        // this is a transition state where we just wait for the user to release the button
        // before moving back to the ONOFF state.
        case TO_ONOFF:
            if ( myVirtualBtn.wasReleased() )
                STATE = ONOFF;
            break;
    }
}

