#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <pthread.h>

class CMutex
{
public:
	CMutex() 
	{
		pthread_mutexattr_t mutex_attr;
		pthread_mutexattr_init(&mutex_attr);
		pthread_mutexattr_settype(&mutex_attr,PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&mMutex, &mutex_attr);
		pthread_mutexattr_destroy(&mutex_attr);
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

