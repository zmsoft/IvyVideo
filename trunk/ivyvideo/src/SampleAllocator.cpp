#include "SampleAllocator.h"
#include "IvyCommon.h"


//
// class CSample
//

CSample::CSample(char *buf, int size) : mBuffer(buf), mBufSize(size)
{
    mData = mBuffer;
    mSize = 0;
}

CSample::~CSample()
{
    if (mBuffer) {
        delete mBuffer;
    }
}

char *CSample::getDataPtr()
{
    return mData;
}

int CSample::getDataSize()
{
    return mSize;
}

bool CSample::setDataSize(int size)
{
    returnf_if_fail (mData != NULL && size <= mBufSize);

    mSize = size;
    return true;
}

bool CSample::setDataPtr(char *data)
{
    returnf_if_fail (mData != NULL && data != NULL);

    mData = data;
    return true;
}

bool CSample::setData(char * data, int len)
{
    returnf_if_fail (mData != NULL && data != NULL);
    returnf_if_fail (mSize + len <= mBufSize);

    memcpy(mData, data, len);
    mSize += len;
    return true;
}

int CSample::getCapacity()
{
    return mBufSize;
}

void CSample::setFormat(int width, int height, int format)
{
    mWidth = width;
    mHeight = height;
    mFormat = format;
}
    

//
// class CSampleAllocator
//

CSampleAllocator *CSampleAllocator::mAllocator = NULL;

CSampleAllocator *CSampleAllocator::inst()
{
    if (mAllocator == NULL) {
        mAllocator = new CSampleAllocator;
    }
    
    return mAllocator;
}

CSample *CSampleAllocator::allocSample(int size)
{
    returnv_if_fail (size > 0 && size <= MAX_SAMPLE_SIZE, NULL);
    
    char * data = new char[size];
    returnv_if_fail(data != NULL, NULL);
    CSample *pSample = new CSample(data, size);
    return pSample;
}

