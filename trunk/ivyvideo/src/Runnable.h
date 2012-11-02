#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

#include <pthread.h>

class CRunnable
{
public:
	CRunnable();
	virtual ~CRunnable();
	
	virtual void startRun();
	virtual void stopRun();
	virtual bool isRunning();
	
	virtual void loopRun() = 0;

protected:
	static void * run(void *param);
	
private:
	enum {
		S_None,
		S_Running,
		S_Stop,
		S_Quit,
	};
	int mStatus;
	pthread_t mPid;
};

#endif
