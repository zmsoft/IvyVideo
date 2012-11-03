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
    CVideoEncode(IvyVideoEncodeSink *sink);
    virtual ~CVideoEncode();

    virtual bool init(int width, int height, int fmt, int fps, int bandwidth);
    virtual void uninit();
    virtual void onRawFrame(char *data, int size, RawFrameFormat format);

protected:
    // for CTimer
    virtual void onTimer();

private:
    CMutex mMutex;
    CSample *mSample;

    // for encode sink
    IvyVideoEncodeSink *mEncodeSink;

    // for ffmpeg
    FFmpegEncoder *mEncoder;
    FFmpegVideoParam mEncodeParam;
};

#endif // _VIDEOENCODE_H_

