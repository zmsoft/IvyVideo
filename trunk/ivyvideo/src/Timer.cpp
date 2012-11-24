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
        mPeriod = 1000; //millisecond
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
    int usec = mPeriod * 1000;
    do{
        this->onTimer();
        usleep(usec);
    }while(isRunning());
}

void CTimer::onTimer()
{}

