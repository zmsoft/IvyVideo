#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

class CRunnable
{
public:
	CRunnable();
	virtual ~CRunnable();
	
	virtual void startRun();
	virtual void stopRun();
	virtual void loopRun();
	virtual bool isRunning();
	
protected:
	static void run(void *param);
	
private:
	enum {
		S_None,
		S_Running,
		S_Stop,
		S_Quit,
	};
	int mStatus;
};

#endif
