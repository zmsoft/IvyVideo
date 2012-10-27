#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <pthread.h>

class CMutex
{
public:
	CMutex() 
	{
		pthread_mutex_init(&mMutex, NULL);
	}

	~CMutex()
	{
		pthread_mutex_destroy(&mMutex);
	}

	void on() { pthread_mutex_lock(&mMutex); }
	void off() { pthread_mutex_unlock(&mMutex); }

private:
	pthread_mutex_t mMutex;
};

class CAutoLock
{
public:
	CAutoLock(CMutex &mtx) : mMutex(mtx) { mMutex.on(); }
	~CAutoLock() { mMutex.off(); }

private:
	CMutex & mMutex;
};

#endif

