/* This file is part of looper library.
   Please check the README file and the notes
   inside the looper.h file
*/


//include required libraries
#include "looper.h"

//global variables
const uint8_t MAX_JOBS = 9; //max allowed jobs -1 (i.e.: 9 = 10-1)

//jobs variables
struct looper_core {
	void (*jobPointer)(void); //store the pointers to user's jobs
	unsigned long userJobInterval; //the interval between each job execution
	unsigned long plannedJob; //keep the next instant of each job execution
	uint8_t jobIsActive;
};
looper_core _jobs[MAX_JOBS];

uint8_t _numJobs = 0; //the number of current running jobs


//class constructor
looper::looper(void) {
}


//add a job to the scheduler
uint8_t looper::addJob(void (*userJob)(void), unsigned long jobInterval, uint8_t oneTimeJob) {
	if (_numJobs == MAX_JOBS) { //max number of allowed jobs reached
		return 1; 
	}

	if ((jobInterval < 1) || (jobInterval > 3600000UL)) { //set your max interval here (max 2^32-1) - default 3600000 (1 hour)
		jobInterval = 50; //50 ms by default
	}
    if (oneTimeJob > 1) { //allowed values are: 0=cyclic job - 1=one_time job
        oneTimeJob = 0;
    }
    //add the job to the scheduler
	_jobs[_numJobs].jobPointer = *userJob;
	_jobs[_numJobs].jobIsActive = oneTimeJob + 1;
	_jobs[_numJobs].userJobInterval = jobInterval;
    _jobs[_numJobs].plannedJob = millis(); // + jobInterval;
	_numJobs++;
    return 0;
}


//pause a specific job
uint8_t looper::pauseJob(void (*userJob)(void)) {
    return (setJob(userJob, 0));
}            
	

//restart a specific job
uint8_t looper::restartJob(void (*userJob)(void)) {
    return (setJob(userJob, 1));
}


//manage the jobs' status
uint8_t looper::setJob(void (*userJob)(void), uint8_t tempStatus, unsigned long jobInterval) {
	//no jobs in the list
    if (_numJobs == 0) {
		return 1;
	}
    //check the jobs in the list
	uint8_t tempI = 0;
	do {
        if (_jobs[tempI].jobPointer == *userJob) { //I've found the job
            _jobs[tempI].jobIsActive = tempStatus; //set the new status
            if (tempStatus == 1) { 
				//set the new interval
				if (jobInterval == 0) {
					_jobs[_numJobs].plannedJob = millis() + _jobs[tempI].userJobInterval;
					
				} else {
					_jobs[_numJobs].plannedJob = millis() + jobInterval;
				}
			}
            break;
        } else {
            tempI++;
    }
	} while (tempI <= _numJobs);
    return 0;
}    


//remove a job from the scheduler
uint8_t looper::removeJob(void (*userJob)(void)) {
	//no jobs in the list
	if (_numJobs == 0) {
		return 1;
	}
    
	//check the list looking for the job
	uint8_t tempI = 0;
	do {
		if (_jobs[tempI].jobPointer == *userJob) { //found it!
			//if it's the last job of the list, I simply decrement the # of jobs
            if ((tempI + 1) == _numJobs) { 
                _numJobs--;
			//otherwise I check if there are more than 1 jobs
            } else if (_numJobs > 1) {
				//move the jobs to 1 position to the left
                for (uint8_t tempJ = tempI; tempJ < _numJobs; tempJ++) {
                    _jobs[tempJ].jobPointer = _jobs[tempJ + 1].jobPointer;
                    _jobs[tempJ].jobIsActive = _jobs[tempJ + 1].jobIsActive;
                    _jobs[tempJ].userJobInterval = _jobs[tempJ + 1].userJobInterval;
                    _jobs[tempJ].plannedJob = _jobs[tempJ + 1].plannedJob;
                }
                _numJobs -= 1;
            } else {
				//or not.. in this case, I simply set the # of jobs to 0
                _numJobs = 0;
            }
			break;
		} else {
			tempI++;
		}
	} while (tempI <= _numJobs);
    return 0;
}

        
//this is the simple scheduler that checks if a routine must be executed
//(code fixing by Dave Parson on 2014/08/05)
void looper::scheduler() {

  //check if the jobs list is empty
  if (_numJobs == 0) {
    return;
  }
  
  unsigned long _tempMillis = millis();
  uint8_t tempI = 0;
  void (*savedJobPointer)(void);
  
  do {
    if (_jobs[tempI].jobIsActive > 0 ) { //the job is running  
      if (_tempMillis - _jobs[tempI].plannedJob > _jobs[tempI].userJobInterval) { //time has come to make it run!
        if (_jobs[tempI].jobIsActive == 2) { //one-time job, we'll run and then remove it
          savedJobPointer = _jobs[tempI].jobPointer;
          savedJobPointer(); //call the job
          tempI = 0; //re-determine the job's position in the array in case it has changed
          do {
            if (_jobs[tempI].jobPointer == savedJobPointer) //found it
              break;
            else
              tempI++;
          } while (tempI <= _numJobs);
          removeJob(savedJobPointer); // remove the one-time job
        } else { //normal job
          _jobs[tempI].jobPointer(); //call the job
          _jobs[tempI].plannedJob = _tempMillis; //set the next time to execute it
        }
      }
    }
    tempI++;
  } while (tempI < _numJobs);
}


//new "delay" function that waits for a certain amount of time while
//continuing to execute the scheduled jobs
void looper::myDelay(unsigned long myPause) {
	unsigned long _tMillis = millis();
	while (millis() - _tMillis < myPause) {
		scheduler();
	}
}
		
