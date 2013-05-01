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
    if (_numJobs == 0) {
		return 1;
	}

	uint8_t tempI = 0;
	do {
        if (_jobs[tempI].jobPointer == *userJob) {
            _jobs[tempI].jobIsActive = tempStatus;
            if (tempStatus == 1) {
				if (jobInterval == 0) {
					_jobs[tempI].plannedJob = millis() + _jobs[tempI].userJobInterval;
				} else {
					_jobs[tempI].plannedJob = millis() + jobInterval;
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
	if (_numJobs == 0) {
		return 1;
	}

	uint8_t tempI = 0;
	do {
		if (_jobs[tempI].jobPointer == *userJob) {
            if ((tempI + 1) == _numJobs) {
                _numJobs--;
            } else if (_numJobs > 1) {
                for (uint8_t tempJ = tempI; tempJ < _numJobs; tempJ++) {
                    _jobs[tempJ].jobPointer = _jobs[tempJ + 1].jobPointer;
                    _jobs[tempJ].jobIsActive = _jobs[tempJ + 1].jobIsActive;
                    _jobs[tempJ].userJobInterval = _jobs[tempJ + 1].userJobInterval;
                    _jobs[tempJ].plannedJob = _jobs[tempJ + 1].plannedJob;
                }
                _numJobs -= 1;
            } else {
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
void looper::scheduler() {

    unsigned long _tempMillis = millis();
	uint8_t tempI = 0;
	do {
		if (_jobs[tempI].jobIsActive > 0 ) { //the job is running
			if (_tempMillis - _jobs[tempI].plannedJob > _jobs[tempI].userJobInterval) { //time has come to get run it away!
                if (_jobs[tempI].jobIsActive == 2) { //this is a one-time job
					_jobs[tempI].jobPointer(); //call the job
                    if ((tempI + 1) == _numJobs) {
                        _numJobs--;
                    } else if (_numJobs > 1) {
                        for (uint8_t tempJ = tempI; tempJ < _numJobs; tempJ++) {
                            _jobs[tempJ].jobPointer = _jobs[tempJ + 1].jobPointer;
                            _jobs[tempJ].jobIsActive = _jobs[tempJ + 1].jobIsActive;
                            _jobs[tempJ].userJobInterval = _jobs[tempJ + 1].userJobInterval;
                            _jobs[tempJ].plannedJob = _jobs[tempJ + 1].plannedJob;
                        }
                        _numJobs -= 1;
                    } else {
                        _numJobs = 0;
                    }
                } else {
                    _jobs[tempI].plannedJob = _tempMillis;
                    _jobs[tempI].jobPointer(); //call the job
                }
			}
		}
	tempI++;
	} while (tempI < _numJobs);
}


//new delay function that wait for a certain amount of time while
//continuing to execute the scheduled jobs
void looper::myDelay(unsigned long myPause) {
	unsigned long _tMillis = millis();
	while (millis() - _tMillis < myPause) {
		scheduler();
	}
}

