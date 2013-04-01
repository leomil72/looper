/*
This is a simple sketch to demonstrate the use of looper, a
simple routines' executor

looper is a library for Atmel microcontrollers and Arduino boards that
starts user's routines at fixed intervals. It doesn't require any interrupt
but just relies inside the loop() function of your sketch.

This sketch demonstrates the blink of 1 LED without using millis() nor delay().
Simply upload the sketch and look at the LED on pin 13.

More info on the schedulers and the methods can be found into the README file.

Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
	
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

*/

#include "looper.h"

looper myScheduler; //create a new istance of the class leOS

//variabiles to control the LEDs
byte led1Status = 0;
const byte LED1 = 13;

//program setup
void setup() {
    pinMode(LED1, OUTPUT);
    
    //add the tasks at the scheduler
    myScheduler.addJob(flashLed1, 1000);
}


//main loop
void loop() {
    myScheduler.scheduler();
}


//flashing task
void flashLed1() {
    led1Status^=1;
    digitalWrite(LED1, led1Status);
}
