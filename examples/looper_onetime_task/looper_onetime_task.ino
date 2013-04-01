/*
This is a simple sketch to demonstrate the use of looper, a
simple routines' executor

looper is a library for Atmel microcontrollers and Arduino boards that
starts user's routines at fixed intervals. It doesn't require any interrupt
but just relies inside the loop() function of your sketch.

This sketch tests the use of one-time tasks. A one-time task is a task that has
to run only once after a specific interval. To try this sketch, create a circuit on a 
protoboard connecting 3 LEDs (and their resistors) at Arduino pins 7, 8, 
and 9. First, a one-time task will light on a LED, then another one-time task
will light it off.

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

//include the scheduler
#include "looper.h"

looper myScheduler; //create a new istance of the class looper

//variabiles to control the LEDs
byte led1Status = 0;
byte led2Status = 0;
const byte LED1 = 7;
const byte LED2 = 8;
const byte LED3 = 9;
byte counter = 10;


//program setup
void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    //add the tasks at the scheduler
    myScheduler.addJob(flashLed1, 2000);
    myScheduler.addJob(lightOnLed3, 5000, ONE_TIME); //one-time task
    myScheduler.addJob(lightOffLed3, 10000, ONE_TIME); //one-time task
}


//main loop
void loop() {
    digitalWrite(LED2, led2Status);
    led2Status ^= 1;    
    delay(1000);
    myScheduler.scheduler();
}


//first task
void flashLed1() {
    led1Status^=1;
    digitalWrite(LED1, led1Status);
}


//second task: a one-time task
//it will light on the LED3
void lightOnLed3() {
    digitalWrite(LED3, HIGH);
}


//third task: another one-time task
//it will light off the LED3
void lightOffLed3() {
    digitalWrite(LED3, LOW);
}
