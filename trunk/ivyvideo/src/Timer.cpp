#include "Timer.h"

#include <unistd.h>
	
CTimer::CTimer()
{
	mPeriod = 0;
}

CTimer::~CTimer()
{
	stopTimer();
}

void CTimer::startTimer(int msec)
{
	mPeriod = msec;
	if (mPeriod <= 0) {
		mPeriod = 1000;
	}
	startRun();
}

void CTimer::stopTimer()
{
	stopRun();
}
	
// for CRunnable
void CTimer::loopRun()
{
	do{
		this->onTimer();
		usleep(mPeriod);
	}while(isRunning());
}

void CTimer::onTimer()
{}
