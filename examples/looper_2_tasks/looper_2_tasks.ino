/*
This is a simple sketch to demonstrate the use of looper, a
simple routines' executor

looper is a library for Atmel microcontrollers and Arduino boards that
starts user's routines at fixed intervals. It doesn't require any interrupt
but just relies inside the loop() function of your sketch.

This sketch tests the use of 2 tasks that run in parallel to the main loop
and the pause & resume methods. To try this sketch, create a circuit on a 
protoboard connecting 3 LEDs (and their resistors) at Arduino pins 7, 8, 
and 9 and then enjoy!

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
byte led2Status = 0;
byte led3Status = 0;
const byte LED1 = 7;
const byte LED2 = 8;
const byte LED3 = 9;
byte counter = 10;
char direction = -1;


//program setup
void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    //add the tasks at the scheduler
    myScheduler.addJob(flashLed1, 2000);
    myScheduler.addJob(flashLed2, 4000);
}


//main loop
void loop() {
    digitalWrite(LED2, led2Status);
    led2Status ^= 1;    
    delay(1000);
    
    //every 10 secs pause/restart the second task
    counter += direction;
    if (counter == 0) {
        myScheduler.pauseJob(flashLed1); //pause the task
        direction = 1;
    }
    if (counter == 10) {
        myScheduler.restartJob(flashLed1); //restart the task
        direction = -1;
    }
    myScheduler.scheduler();
}


//first task
void flashLed1() {
    led1Status^=1;
    digitalWrite(LED1, led1Status);
}


//second task
void flashLed2() {
    led3Status^=1;
    digitalWrite(LED3, led3Status);
}
