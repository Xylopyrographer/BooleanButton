// Boolean Button Library
// https://github.com/Xylopyrographer/BooleanButton
// Copyright (C) 2023 by Xylopyrographer and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#include "BooleanButton.h"

/*----------------------------------------------------------------------*
/ initialise a BoolBtn object
/-----------------------------------------------------------------------*/
void BoolBtn::begin() {
    m_time = millis();
    m_state = *m_bool;
    if ( m_invert )
        m_state = !m_state;
    m_lastState = m_state;
    m_lastTransient = !m_state;
    m_changed = false;
    m_stable = false;
    m_lastChange = m_time;
    m_lastDbTime = m_time;
}

/*----------------------------------------------------------------------*
/ returns the state of the variable, inverted if set as such during
/ instantiation. does debouncing, captures and maintains times, previous
/ state, etc.
/-----------------------------------------------------------------------*/
bool BoolBtn::read() {
    uint32_t ms = millis();
    bool boolVal = *m_bool;
    if ( m_invert )
        boolVal = !boolVal;
    if ( boolVal != m_lastTransient ) {
    	m_lastDbTime = ms;
    	m_lastTransient = boolVal;
    	m_stable = false;
        m_changed = false;
    }
	if ( ( ms - m_lastDbTime ) > m_dbTime ) {
		m_lastState = m_state;
		m_state = boolVal;
		m_stable = true;
		m_changed = ( m_state != m_lastState );
		if (m_changed)
			m_lastChange = ms;
	}
	m_time = ms;
    return m_state;
}

/*----------------------------------------------------------------------*
/ isPressed() and isReleased() check the variable state when it
/ was last read, and return false (0) or true (!=0) accordingly.
/ these functions do not cause the variable to be read.
-----------------------------------------------------------------------*/
bool BoolBtn::isPressed() {
    return m_state;
}

bool BoolBtn::isReleased() {
    return !m_state;
}

/*----------------------------------------------------------------------*
/ wasPressed() and wasReleased() check the variable state to see
/ if it changed between the last two reads and returns false (0) or
/ true (!=0) accordingly.
/ these functions do not cause the variable to be read.
-----------------------------------------------------------------------*/
bool BoolBtn::wasPressed() {
    return m_state && m_changed;
}

bool BoolBtn::wasReleased() {
    return !m_state && m_changed;
}

/*----------------------------------------------------------------------*
/ pressedFor(ms) and releasedFor(ms) check to see if the variable is
/ true (or false), and has been in that state for the specified
/ time in milliseconds. Returns false (0) or true (!=0) accordingly.
/ these functions do not cause the variable to be read.
-----------------------------------------------------------------------*/
bool BoolBtn::pressedFor( uint32_t ms ) {
    return m_state && m_time - m_lastChange >= ms;
}

bool BoolBtn::releasedFor( uint32_t ms ) {
    return !m_state && m_time - m_lastChange >= ms;
}

/*----------------------------------------------------------------------*
/ isStable() returns true if the variable has not changed state
/ for at least debounce time, false otherwise.
/ does not cause the variable to be read.
-----------------------------------------------------------------------*/
bool BoolBtn::isStable() {
    return m_stable;
}

/*----------------------------------------------------------------------*
/ lastChange() returns the time the debounced variable last changed
/ state, in milliseconds.
-----------------------------------------------------------------------*/
uint32_t BoolBtn::lastChange() {
    return m_lastChange;
}
