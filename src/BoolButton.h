// Boolean Button Library
// https://github.com/Xylopyrographer/BooleanButton
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#ifndef BOOL_BUTTON_H_INCLUDED
#define BOOL_BUTTON_H_INCLUDED

#include <Arduino.h>

class BoolBtn {
public:
    // BoolBtn( theBool, dbTime, invert ) instantiates a boolean button object.
    //
    // Required parameters:
    // theBool  Pointer (address) to the boolean variable being monitored.
    //
    // Optional parameters:
    // dbTime   Debounce time in milliseconds (default 25ms)
    // invert   True to interpret a boolean value of false as true (default false)
    BoolBtn( bool* theBool, uint32_t dbTime = 25, bool invert = false )
        : m_bool( theBool ), m_dbTime( dbTime ), m_invert( invert ) {}

    // Initialize a BoolBtn object.
    void begin();

    // Returns the current debounced variable state, true for pressed,
    // false for released. Call this function frequently to ensure
    // the sketch is responsive to changes in the variable.
    bool read();

    // Returns true if the variable state is true at the last call to read().
    // Does not cause the variable to be read.
    bool isPressed();

    // Returns true if the variable state is false at the last call to read().
    // Does not cause the variable to be read.
    bool isReleased();

    // Returns true if the variable state at the last call to read() is true,
    // and this was a change since the previous read.
    bool wasPressed();

    // Returns true if the variable state at the last call to read() is false,
    // and this was a change since the previous read.
    bool wasReleased();

    // Returns true if the variable state at the last call to read() was true,
    // and has been in that state for at least the given number of milliseconds.
    bool pressedFor( uint32_t ms );

    // Returns true if the variable state at the last call to read() was false,
    // and has been in that state for at least the given number of milliseconds.
    bool releasedFor( uint32_t ms );

    // Returns the time in milliseconds (from millis) that the variable last
    // changed state.
    uint32_t lastChange();

private:
    bool* m_bool;           // pointer to the boolean variable being monitored
    uint32_t m_dbTime;      // debounce time (ms)
    bool m_invert;          // if true, interpret a variable state of true as false and a state of false as true
    bool m_state;           // current variable state
    bool m_lastState;       // previous variable state
    bool m_changed;         // true if the variable state changed since last read
    uint32_t m_time;        // time of current state (ms, from millis())
    uint32_t m_lastChange;  // time of last state change (ms)
};

#endif
