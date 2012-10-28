#include "timer.h"

CRunnable::CRunnable()
{
	mStatus = S_None;
}

CRunnable::~CRunnable()
{
}

bool CRunnable::isRunning() 
{ 
	return (mStatus == S_Running); 
}

void CRunnable::startRun()
{
	mStatus = S_Running;
	//pthread_create(run, this);
}

void CRunnable::stopRun()
{
	mStatus = S_Stop;
}

void CRunnable::loopRun()
{
}

void CRunnable::run(void *param)
{
	CRunnable *p = (CRunnable *)param;
	if (p) {
		p->loopRun();
	}
	mStatus = S_Quit;
}

