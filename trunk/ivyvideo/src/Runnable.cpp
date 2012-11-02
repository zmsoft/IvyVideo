#include "Runnable.h"

CRunnable::CRunnable()
{
	mStatus = S_None;
	mPid = (pthread_t)-1;
}

CRunnable::~CRunnable()
{
	mStatus = S_None;
}

bool CRunnable::isRunning() 
{ 
	return (mStatus == S_Running); 
}

void CRunnable::startRun()
{
	mStatus = S_Running;
	pthread_create(&mPid, NULL, run, this);
}

void CRunnable::stopRun()
{
	mStatus = S_Stop;
}

void *CRunnable::run(void *param)
{
	CRunnable *p = (CRunnable *)param;
	if (p) {
		p->loopRun();
		p->mStatus = S_Quit;
	}

	return NULL;
}

