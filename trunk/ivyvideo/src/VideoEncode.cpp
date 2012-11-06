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

bool CVideoEncode::init(int width, int height, int fmt, int fps, int bandwidth, const char *codec)
{
    LOGI("CVideoEncode.init() begin, codec: %s", codec);

    int pixFmt;
    returnf_if_fail(getPixelFormat(fmt, pixFmt));

    // init with defalut parameters
    mEncodeParam.setVideoParam(width, height, (PixelFormat)pixFmt, bandwidth, fps, codec);
    mEncoder = new FFmpegEncoder(mEncodeParam);

    if(mEncoder->open() != 0) {
        LOGE("CVideoEncode.init(), failed to open FFmpegEncoder: %s", codec);
        delete mEncoder;
        mEncoder = NULL;
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
    LOGI("CVideoEncode.onRawFrame() begin, encode[%d, %d], input[%d, %d], len[%d]",
        mEncodeParam.width, mEncodeParam.height, format.width, format.height, size);
    return_if_fail(format.fmt == ANDROID_NV21);

    CAutoLock lock(mMutex);
    if (mSample) {
        if (mSample->getCapacity() < size) {
            mSample->release();
            mSample = NULL;
        }
    }

    if (mSample == NULL) {
        mSample = CSampleAllocator::inst()->allocSample(size);
        return_if_fail(mSample != NULL);
        mSample->addRef();
    }
    
    return_if_fail(mSample->setDataSize(size));
    NV21toI420(data, mSample->getDataPtr(), format.width, format.height, 1);
    format.fmt = CSP_I420;
    mSample->setFormat(format.width, format.height, format.fmt);
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

    return_if_fail(pSample != NULL);

    LOGI("CVideoEncode::onTimer, get one frame and encode it");
    int size = mEncoder->encodeVideoFrame((const uint8_t *)pSample->getDataPtr());
    if (size > 0) {
        // maybe rtp pack
        if (mEncodeSink) {
            mEncodeSink->onPacked((char *)mEncoder->getVideoEncodedBuffer(), size, PT_RAW);
        }
    }
    pSample->release();
}

