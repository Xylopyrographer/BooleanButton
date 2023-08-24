// Boolean Button Library
// https://github.com/Xylopyrographer/BooleanButton
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch that uses both short and long button presses to adjust
// a number up and down, between two limits. Short presses increment
// or decrement by one, long presses repeat at a specified rate.
// Every time the number changes, it is written to the serial monitor.

#include <BoolButton.h>      // https://github.com/Xylopyrographer/BooleanButton

// pin assignments
//  - change the pin numbers to match your wiring
#define UP_PIN  14              // connect a momentary push button switch from this pin to ground
#define DN_PIN  27              // ditto

bool boolUP = 0;                // the variables to monitor
bool boolDN = 0;

// define the BoolBtn objects (note: we need to pass the address (a pointer)
// to the variable, so need to prefix the variable name with '&')
BoolBtn virtualBtnUP( &boolUP, 25, true );      // 'invert' is set to true as we're using a button wired to ground
BoolBtn virtualBtnDN( &boolDN, 25, true );

#define REPEAT_FIRST    750     // ms required before repeating on long press
#define REPEAT_INCR     100     // repeat interval for long press

#define MIN_COUNT   0
#define MAX_COUNT   59

void setup() {
    Serial.begin( 115200 );
    delay( 250 );

    pinMode( UP_PIN, INPUT_PULLUP );    // set the button pin as an input
    pinMode( DN_PIN, INPUT_PULLUP );    // ditto

    boolUP = digitalRead( UP_PIN );     // set the initial state of the variables being monitored
    boolDN = digitalRead( DN_PIN );

    virtualBtnUP.begin();               // initialize the BoolBtn objects
    virtualBtnDN.begin();

    virtualBtnUP.read();                // set the initial state of the variables being monitored inside the object.
    virtualBtnDN.read();
}

void loop() {
    // get the current state of the buttons into the boolean variables being monitored
    boolUP = digitalRead( UP_PIN ); 
    boolDN = digitalRead( DN_PIN );

    virtualBtnUP.read();                // read the states of the variables into the BoolBtn objects
    virtualBtnDN.read();

    static int
        count,                          // the number that is adjusted
        lastCount( -1 );                // previous value of count (initialized to ensure it's different when the sketch starts)
    static unsigned long
        rpt( REPEAT_FIRST );            // a variable time that is used to drive the repeats for long presses
    enum states_t { WAIT, INCR, DECR }; // states for the state machine
    static states_t STATE;              // current state machine state

    if ( count != lastCount ) {         // print the count if it has changed
        lastCount = count;
        Serial.println( count, DEC );
    }

    switch ( STATE ) {
        case WAIT:                              // wait for a button event
            if ( virtualBtnUP.wasPressed() )
                STATE = INCR;
            else if ( virtualBtnDN.wasPressed() )
                STATE = DECR;
            else if ( virtualBtnUP.wasReleased() )     // reset the long press interval
                rpt = REPEAT_FIRST;
            else if ( virtualBtnDN.wasReleased() )
                rpt = REPEAT_FIRST;
            else if ( virtualBtnUP.pressedFor( rpt ) ) { // check for long press
                rpt += REPEAT_INCR;             // increment the long press interval
                STATE = INCR;
            }
            else if ( virtualBtnDN.pressedFor( rpt ) ) {
                rpt += REPEAT_INCR;
                STATE = DECR;
            }
            break;

        case INCR:
            ++count;                            // increment the counter
            count = min( count, MAX_COUNT );    // but not more than the specified maximum
            STATE = WAIT;
            break;

        case DECR:
            --count;                            // decrement the counter
            count = max( count, MIN_COUNT );    // but not less than the specified minimum
            STATE = WAIT;
            break;
    }
}
