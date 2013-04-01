/*
	looper - a simple routines' launcher
    
	looper is a library for Atmel microcontrollers and Arduino boards that 
    starts user's routines (or "jobs") at fixed intervals. It doesn't require
    any interrupt but just relies inside the loop() function of your sketch
    and uses millis() to execute the jobs (it doesn't suffer the millis()
    overflow issue).

	Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
	
	Current versin:
	v. 1.0:  millis overflow management - new method syntaxes to be compatible with leOS

	The version history is available in the README file.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public
	License as published by the Free Software Foundation; either
	version 3.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.

*/


#ifndef looper_H
#define looper_H

//library version
#define looper_VERSION 100

//Library is compatible both with Arduino <=0023 and Arduino >=100
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const uint8_t ONE_TIME = 1;

class looper {
	public: 
		//public methods
		looper();
        //void begin(void);
        void (*voidFuncPtr)(void);
		uint8_t addJob(void (*)(void), unsigned long, uint8_t oneTimeJob = 0);
		uint8_t removeJob(void (*)(void));
		uint8_t pauseJob(void (*)(void));
        uint8_t restartJob(void (*)(void));
		void scheduler(void);
		void myDelay(unsigned long myPause = 1);
	private:
        //private methods
        uint8_t setJob(void (*)(void), uint8_t, unsigned long jobInterval = 0);
};

#endif
