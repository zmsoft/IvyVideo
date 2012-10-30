#include "VideoEncode.h"

CVideoEncode::CVideoEncode()
{
	mSample = NULL;
	mEncodeSink = NULL;
	mEncoder = NULL;
	mParam = NULL;
}

CVideoEncode::~CVideoEncode()
{
	uninit();
}

bool CVideoEncode::init()
{
	// init with defalut parameters
	mParam = new FFmpegVideoParam(320, 180, PIX_FMT_YUV420P, 500000, 25);
	FFmpegAudioParam audioNull;
	mEncoder = new FFmpegEncoder(*mParam, audioNull);
	
	if(mEncoder->open() != 0) {
		return false;
	}
	
	return true;
}

void CVideoEncode::uninit()
{
	if (mEncoder) {
		mEncoder->close();
		delete mEncoder;
	}
	
	if (mParam) {
		delete mParam;
	}
}

void CVideoEncode::onRawFrame(char *data, int size, int width, int height, int format)
{
	return_if_fail(mParam != NULL);
	return_if_fail(width == mParam->width && height == mParam->height);
	
	mMutex.on();
	if (mSample == NULL) {
		mSample = CSampleAllocator::inst()->allocSample(0);
		mSample->setFormat(width, height, format);
	}
	
	mSample->setData(data, size);
	mMutex.off();
}

// 
// for encoding process
void CVideoEncode::onTimer()
{
	return_if_fail(mEncoder != NULL);

	CSample *pSample = NULL;
	mMutex.on();
	if (mSample) {
		pSample = mSample;
		mSample = NULL;
		pSample->addRef();
	}
	mMutex.off();
	
	if (pSample) {
		int size = mEncoder->encodeVideoFrame((const uint8_t *)pSample->getDataPtr());
		if (size > 0) {
			// maybe rtp pack
			if (mEncodeSink) {
				mEncodeSink->onPacked((char *)mEncoder->getVideoEncodedBuffer(), size, PT_RAW);
			}
		}
		
		pSample->release();
	}
}


