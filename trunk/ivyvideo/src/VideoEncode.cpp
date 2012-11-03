#include "VideoEncode.h"
#include "IvyUtil.h"
#include "LogTrace.h"

CVideoEncode::CVideoEncode(IvyVideoEncodeSink *sink) : mEncodeSink(sink)
{
    mSample = NULL;
    mEncoder = NULL;
}

CVideoEncode::~CVideoEncode()
{
    uninit();
}

bool CVideoEncode::init(int width, int height, int fmt, int fps, int bandwidth)
{
    LOGI("CVideoEncode.init() begin");

    int pixFmt;
    return_val_if_fail(getPixelFormat(fmt, pixFmt), false);

    // init with defalut parameters
    mEncodeParam.setVideoParam(width, height, (PixelFormat)pixFmt, bandwidth, fps);
    FFmpegAudioParam audioNull;
    mEncoder = new FFmpegEncoder(mEncodeParam, audioNull);

    if(mEncoder->open() != 0) {
        LOGE("FFmpegEncoder.open() failed");
        return false;
    }

    LOGI("CVideoEncode.init() end");
    return true;
}

void CVideoEncode::uninit()
{
    LOGI("CVideoEncode.uninit() begin");
    mMutex.on();

    if (mEncoder) {
        mEncoder->close();
        delete mEncoder;
        mEncoder = NULL;
    }

    mMutex.off();
    LOGI("CVideoEncode.uninit() end");
}

void CVideoEncode::onRawFrame(char *data, int size, RawFrameFormat format)
{
    return_if_fail(format.width == mEncodeParam.width && 
        format.height == mEncodeParam.height);

    mMutex.on();
    if (mSample == NULL) {
        mSample = CSampleAllocator::inst()->allocSample(0);
        mSample->setFormat(format.width, format.height, format.fmt);
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


