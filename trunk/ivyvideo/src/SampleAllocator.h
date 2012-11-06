#ifndef _SAMPLEALLOCATOR_H_
#define _SAMPLEALLOCATOR_H_

#include "IDummyObject.h"

class CSample : public IDummyObject
{
public:
    CSample(char *buf, int size);
    virtual ~CSample();

    char *getDataPtr();
    int getDataSize();

    bool setDataPtr(char *data);
    bool setDataSize(int size);

    bool setData(char *data, int size);

    int getCapacity();

    void setFormat(int width, int height, int format);
private:
    char *mBuffer;
    int  mBufSize; 

    char *mData;
    int  mSize;

    // for video
    int mWidth;
    int mHeight;
    int mFormat;
};

class CSampleAllocator
{
public:
    static CSampleAllocator *inst();
    CSample *allocSample(int size);

private:
    static CSampleAllocator *mAllocator;
};

#endif

