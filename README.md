# BooleanButton Library
https://github.com/Xylopyrographer/BooleanButton  
README file  

## License
BooleanButton Library Copyright (C) 2023 Xylopyrographer, released under GNU GPL v3.0

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License v3.0 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/gpl.html>

## Introduction
The BooleanButton library is for monitoring the state of a boolean variable, treating that variable as if it were a physical button (creates a virtual button if you like).  "Long presses" of arbitrary length can be detected. Works well in state machine constructs.  Use the `read()` function to read each monitored variable in the main loop, which should execute as fast as possible.

Why would you want this? While doing some work on controlling the backlight of an LED display with an embedded capacitive touch panel, I noted the results returned from the touch panel driver library were "twitchy", depending on how the panel was touched. The panel was in essence acting like a physical momentary contact push button. Rather than re-invent the wheel, and having experience with the very excellent [JC\_Button](https://github.com/JChristensen/JC_Button) library, I modified that so it could treat any boolean variable as a virtual momentary push button.

Thus, if you have a need to monitor the state of a boolean — how long it's been in a state, when last changed, its current state, etc. this may just be the library you need.

## Examples
The following example sketches are included with the **BooleanButton** library:

- **SimpleOnOff**: Turns an LED on and off.
- **LongPress**: Demonstrates detecting long and short variable changes.
- **UpDown**: Counts up or down, one number at a time or rapidly by monitoring the states of two variables.

## Constructor

### BoolBtn( theBoolean, dbTime, invert )
##### Description
The constructor defines a virtual boolean button object.
##### Syntax
`BoolBtn( theBoolean, dbTime, invert );`
##### Required parameter
**theBoolean:** pointer (address) to a boolean variable to be monitored.  *(type = \* bool)*  
##### Optional parameters
**dbTime:** Debounce time in milliseconds. Defaults to 25ms if not given. *(type = unsigned long)*  
**invert:** *false* reverses the interpretation of the variable being monitored; true is interpreted as false and vice versa. Defaults to false if not given. *(type = bool)*
##### Returns
None.
##### Examples
```arduino
// variable "myVar" is to be monitored, 25ms debounce, logic not inverted
bool myVar;
BoolBtn theVariable( &myVar );

// same as above but needs a longer debounce time (50ms)
bool myVar;
BoolBtn theVariable( &myVar, 50 );

// monitor "myVar", with a 25ms debounce time and invert the logic
bool theVariable;
BoolBtn theVariable( & myVar, 25, true );

```

## BooleanButton Library Functions

### begin()
##### Description
Initializes the BoolBtn object.
##### Syntax
`theVariable.begin();`
##### Parameters
None.
##### Returns
None.
##### Example
```arduino
theVariable.begin();
```
### read()
##### Description
Reads and returns the state of the variable being monitored and returns a *boolean* value (*true* or *false*) accordingly.

The read() function needs to execute very frequently in order for the sketch to be responsive. A good place for `read()` is at the top of `loop()`. Often, the return value from `read()` will not be needed if the other functions below are used.
##### Syntax
`theVariable.read();`
##### Parameters
None.
##### Returns
*true* if the variable is true, else *false*.  *(type = bool)*
##### Example
```arduino
theVariable.read();
```

### isPressed()<br>isReleased()
##### Description
These functions check the variable state as found from the last call to `read()` and returns false or true accordingly. These functions *do not* cause the variable to be read.
##### Syntax
`theVariable.isPressed();`  
`theVariable.isReleased();`
##### Parameters
None.
##### Returns
*true* or *false*, depending on whether the variable is true (false) or not. *(type = bool)*
##### Example
```arduino
if ( theVariable.isPressed() ) {
	//do something
}
else {
	//do something else
}
```

### wasPressed()<br>wasReleased()
##### Description
These functions check the variable state to see if it changed between the last two calls to `read()` and returns false or true accordingly. These functions *do not* cause the variable to be read. Note that these functions may be more useful than `isPressed()` and `isReleased()` since they actually detect a *change* in the state of the variable, which is usually what we want in order to cause some action.
##### Syntax
`theVariable.wasPressed();`  
`theVariable.wasReleased();`
##### Parameters
None.
##### Returns
*true* or *false*, depending on whether the button was pressed (released) or not. *(type = boolean)*.
##### Example
```arduino
if ( theVariable.wasPressed() ) {
	//do something
}
```

### pressedFor(ms)<br>releasedFor(ms)
##### Description
These functions check to see if the variable is true (or false), and has been in that state for the specified time in milliseconds. Returns true or false accordingly. These functions are useful to detect "long presses". Note that these functions *do not* cause the variable to be read.
##### Syntax
`theVariable.pressedFor(ms);`  
`theVariable.releasedFor(ms);`
##### Parameters
**ms:** The number of milliseconds *(unsigned long)*
##### Returns
*true* or *false*, depending on whether the variable is true (false) for the specified time. *(type = bool)*
##### Example
```arduino
if ( theVariable.pressedFor(1000) ) {
    // variable has been true for 1000 ms
}
```

### lastChange()
##### Description
Under certain circumstances, it may be useful to know when a variable last changed state. `lastChange()` returns the time the variable last changed state, in milliseconds (the value is derived from the Arduino `millis()` function).
##### Syntax
`theVariable.lastChange();`
##### Parameters
None.
##### Returns
The time in milliseconds when the variable last changed state. *(type = unsigned long)*
##### Example
```arduino
unsigned long msLastChange = theVariable.lastChange();
```

## Credit
BooleanButton is a virtual clone of the JC\_Button library written by Jack Christensen, available at [https://github.com/JChristensen/JC_Button](https://github.com/JChristensen/JC_Button). This work is fully acknowledged and credited.

Since BooleanButton creates a virtual physical button from a boolean variable, the syntax of JC\_Button was retained — hopefully to anchor the concept.

Many thanks Jack!

