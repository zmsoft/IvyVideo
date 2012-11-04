#ifndef _IDUMMYOBJECT_H_
#define _IDUMMYOBJECT_H_

#include "MutexLock.h"

class IDummyObject
{
public:
    IDummyObject() : mRefCount(0){}
    virtual ~IDummyObject() {}

    virtual void addRef()
    {
        mMutex.on();
        mRefCount++;
        mMutex.off();
    }

    virtual void release()
    {
        mMutex.on();
        mRefCount --;
        if (mRefCount <= 0) {
            delete this;	
        }
        mMutex.off();
    }

private:
    int mRefCount;
    CMutex mMutex;
};

#endif

