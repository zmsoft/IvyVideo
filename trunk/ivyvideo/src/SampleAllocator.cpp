#include "SampleAllocator.h"
#include "IvyVideo.h"


//
// class CSample
//

CSample::CSample(char *buf, int size) : mBuffer(buf), mBufSize(size)
{
	mData = NULL;
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

void CSample::setFormat(int width, int height, int format)
{
	mWidth = width;
	mHeight = height;
	mFormat = format;
}

bool CSample::setData(char *data, int size)
{
	if (data == NULL || size > mBufSize || mBuffer == NULL)
	{
		return false;
	}
	
	memcpy(mBuffer, data, size);
	mData = mBuffer;
	mSize = size;
	
	return true;
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
	if (size <= 0 || size > MAX_SAMPLE_SIZE) 
	{
		return NULL;
	}
	
	char * data = new char[size];
	CSample *pSample = new CSample(data, size);
	return pSample;
}