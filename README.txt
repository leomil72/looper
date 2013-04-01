******** looper.h  *******

looper - a simple scheduler to execute little jobs
  
looper is a library for Atmel microcontrollers and Arduino boards that
starts user's routines at fixed intervals. It doesn't require any interrupt
but just relies inside the loop() function of your sketch.
This is a refreshened version of looper, that has been combined with another
version of the scheduler named looper2 that I only released on the
Arduino forum: loopers2 was able to work together with my interrupt driven
scheduler named leOS due to the fact that I changed the names to the functions
that manage the user's routines, or "jobs" (instead of "tasks"). Moreover,
this new version of looper doesn't suffer the millis() overflow issue.
Enjoy!

Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>

***********************
VERSION HISTORY

v. 1.0:  millis overflow management - new method syntaxes to be compatible with leOS
v. 0.1:  first release


***********************
HOW TO USE IT - METHODS

Unpack the library and copy it into your /libraries folder, that usually is in
your sketchs' folder. Then include the library and create a new istance of it
by adding the following code at the top of your sketch:

#include "looper.h"
looper myScheduler

Now you can add a task by simply call the method .addTask():
void setup() {
  myScheduler.addJob(yourFunction, scheduleTime);
  ...
}

yourFunction must be a routine that is inside your sketch. scheduleTime is
the scheduled interval in milliseconds at which you want your routine
to be executed. 

To start the scheduler to check and execute your jobs you have to put
the call to .scheduler() in any point of the loop() routine.
It is better if you put it at the end of the loop's code:
void loop() {
  ...
  myScheduler.scheduler();
}

To pause a job, just call the following method:
myScheduler.pauseJob(yourFunction);

You can restart it with:
myScheduler.restartJob(yourFunction);

To remove a task from the scheduler call this method:
myScheduler.removeJob(yourFunction);

An interesting feature is the ability to run one-time jobs. A one-time job
is a job that will be run only once. To add a one-time job just add the paramether
ONE_TIME after the interval:
myScheduler.addJob(yourFunction, interval, ONE_TIME);

This will inform the scheduler that once it has been executed, the job must
be removed from the list of the scheduled jobs (it won't be paused, it will
be permanently deleted).

A particular method is myDelay. This method will wait for a certain amount
of time while still continue executing the scheduled jobs.
The use is simple, instead of the Arduino function delay use myDelay:

.....
myScheduler.myDelay(interval);
.....

Where "interval" is the time in milliseconds to wait before to continue.
Keep in mind that myDelay will stop the execution of the main code while
still continues to execute the scheduled jobs.


***********************
HOW IT WORKS

looper does not use timers nor interrupts. It is just a sub-routine that you have
to call and then it makes a control over the jobs that have been scheduled.
Keep in mind that if the loop runs for a time longer than the scheduled time
of your jobs, the scheduler will seem to work incorrectly due to the fact that
the planned intervals won't be respected.


***********************
LICENSE

This library is free software; you can redistribute it and/or modify it under 
the terms of the GNU General Public	License as published by the Free Software 
Foundation; either version 3.0 of the License, or (at your option) any later 
version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.


***********************
DOCUMENT REVISION

2nd revision: 2013/04/01
