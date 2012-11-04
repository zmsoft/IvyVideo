#ifndef _TIMER_H_
#define _TIMER_H_

#include "Runnable.h"

class CTimer : public CRunnable
{
public:
    CTimer();
    virtual ~CTimer();

    void startTimer(int msec);
    void stopTimer();

    virtual void onTimer();

    // for CRunnable
    virtual void loopRun();

private:
    int mPeriod; // ms
};

#endif
