#ifndef _VIDEOENCODE_H_
#define _VIDEOENCODE_H_

#include "IvyCommon.h"
#include "IvyVideo.h"
#include "MutexLock.h"
#include "Timer.h"
#include "SampleAllocator.h"

#include "FFmpegEncoder.h"


class CVideoEncode : 
	public IvyVideoEncode,
	public CTimer,
	public IDummyObject
{
public:
	CVideoEncode();
	virtual ~CVideoEncode();

	virtual bool init();
	virtual void uninit();
	virtual void onRawFrame(char *data, int size, RawFrameFormat format);

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

