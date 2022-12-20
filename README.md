# gLibraries ecosystem

This is a collection of Arduino libraries aimed to simplify the usage of several sensors, it uses a base class and common functions to hide the low level logic from the user, boosting the develop time to get the proyects done faster.

`note`: The collection is dependent of the dynamicVector library, that can be find [here](https://github.com/JhoselinRam/dynamicVector)

The libraries included in this collection are:
___
### hardwareEnclosure
This is the base class of all the other classes in the collection, it allows the initialization and update of all instances of child clases by calling a single function, namely:
>`beginAll();`
>
>`updateAll();`
___
### gButton
This librarie allows the user to use buttons and other momentary contact switches, it provides a debounce functionality and the capability of detecting, pressing, releasing, sustain, click, double click, multy click and more.
All without the use of the delay function, so the program dosn´t halt.
Also allows to use the buttons safely without the use of external resistors.
___
### gLed
This librarie allows the user to implement led's in their proyects whith ease, it includes a variety of functions and animations like blink, fade in/out, pulses, etc.
Providing great control over the animation times, all whitout using the delay function, so the program dosn't halt during the animations. 
___
### gSonar
This librarie provides functionality to use contactless ultrasonic distance sensors, it allows the measurement in diferent units and include a temperature compensation for the velocity of the ultrasonic waves that can be updated on the fly.
Also implements a tunable real time filter to smooth out the readings, raw data can be extracted too.

Currently  only support the `HC-SR04` sensor, more sensors may be supported in the future.
___
### gStepper
This librarie provides functionality to use stepper motors. It allows the user to set the movement of the motor in steps, grades and radiants, it also allows to set the number of steps in the motor and the use of full steps (one and two phases) and half steps.
Currently continuos steps and acceleration are not supported, this functionality may de added in the future.
___
### gVernierA
This librarie provides functionality to interface and extract data from the analog Vernier sensors using the BTA (British Telecom Analog) connector. 
It provides an auto identification of the sensor and implement a real time filter to smooth out the data.
___
### gVernierD
This librarie provides functionality to interface and extract data from the digital Vernier sensors using the BTD (British Telecom Digital) connector. 
It provides an auto identification of the sensor and implement a real time filter to smooth out the data.
___
### gI2C
This auxiliar library provides some functions to interface with the I2C (TWI) bus in a simple manner.
___
### gMatrix
This library implement matrix and matrix algebra to the Arduino platform inspired in the MatLab notacion. Allowing the user to set, compare and operate matrices in a natural way.

All matrices are dinamicaly allocated, so they can shink and grow on demand. This library must be used with caution due the dinamyc allocated memory.
