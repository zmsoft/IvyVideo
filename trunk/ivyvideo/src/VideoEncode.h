#ifndef _VIDEOENCODE_H_
#define _VIDEOENCODE_H_

#include "IvyVideo.h"
#include "MutexLock.h"
#include "Timer.h"
#include "SampleAllocator.h"

#include "FFmpegEncoder.h"


class CVideoEncode : public IDummyObject, public CTimer
{
public:
	CVideoEncode();
	virtual ~CVideoEncode();

	bool init();
	void uninit();

	void onRawFrame(char *data, int size, int width, int height, int format);

protected:
	virtual void onTimer();
	
private:
	IvyVideoEncodeSink *mEncodeSink;
	CSample *mSample;
	CMutex mMutex;
	
	// for ffmpeg
	FFmpegEncoder *mEncoder;
	FFmpegVideoParam *mParam;
};


#endif // _VIDEOENCODE_H_

