#include "VideoEncode.h"
#include "IvyUtil.h"
#include "LogTrace.h"

#define _TEST_ENCODE

#ifdef _TEST_ENCODE
#ifdef ANDROID
#define FRAME_PATH "/sdcard"
#else
#define FRAME_PATH "/tmp"
#endif
#endif


static void write_raw_to_file(char *data, int size)
{
#ifdef _TEST_ENCODE
    if (size <= 0 || !data) {
        return;
    }

    static int once = 23;
    if (once > 0) {
        once --;

        static FILE *fpraw = NULL;
        if (fpraw == NULL) {
            fpraw = fopen(FRAME_PATH"/video_i420.yuv", "wb");
        }
        if (fpraw != NULL) {
            fwrite(data, 1, size, fpraw);
            fflush(fpraw);
            if (once <=0 ) {
                fclose(fpraw);
                fpraw = NULL;
            }
        }
    }
#endif
}

static void write_avc_to_file(char *data, int size)
{
#ifdef _TEST_ENCODE
    if (size <= 0 || !data) {
        return;
    }

    static int once = 120;
    if (once > 0) {
        once --;

        static FILE *fpavc = NULL;
        if (fpavc == NULL) {
            fpavc = fopen(FRAME_PATH"/video_avc.264", "wb");
        }
        if (fpavc != NULL) {
            fwrite(data, 1, size, fpavc);
            fflush(fpavc);
            if (once <= 0) {
                fclose(fpavc);
                fpavc = NULL;
            }
        }
    }
#endif
}

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
    LOGI("CVideoEncode.init() begin, codec: %s, w: %d, h: %d, fmt: %d, fps: %d", 
            codec, width, height, fmt, fps);

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

void CVideoEncode::onRawFrame(char *data, int size, RawFrameFormat &inFmt)
{
    LOGI("CVideoEncode.onRawFrame() begin, encode[%d, %d], input[%d, %d], len[%d]",
        mEncodeParam.width, mEncodeParam.height, inFmt.width, inFmt.height, size);
    return_if_fail(inFmt.fmt == ANDROID_NV21);

    // crop frame
    int frameWidth = mEncodeParam.width;
    int frameHeight = mEncodeParam.height;
    int frameSize = frameWidth * frameHeight * 3 / 2;

    CSample *pSample = CSampleAllocator::inst()->allocSample(frameSize);
    return_if_fail(pSample != NULL);
    pSample->addRef();

    pSample->setDataSize(frameSize);
    bool bret = CropYUVFrame(data, inFmt.width, inFmt.height, CSP_NV21, 
            pSample->getDataPtr(), frameWidth, frameHeight);
    if (!bret) {
        pSample->release();
        LOGE("CVideoEncode.onRawFrame(), failed to CropYUVFrame");
        return;
    }
    write_raw_to_file(pSample->getDataPtr(), pSample->getDataSize());

    CAutoLock lock(mMutex);
    if (mSample) {
        if (mSample->getCapacity() < frameSize) {
            mSample->release();
            mSample = NULL;
        }
    }

    if (mSample == NULL) {
        mSample = CSampleAllocator::inst()->allocSample(frameSize);
        return_if_fail(mSample != NULL);
        mSample->addRef();
    }
    
    mSample->setFormat(frameWidth, frameHeight, CSP_I420);
    return_if_fail(mSample->setDataSize(frameSize));
    return_if_fail(NV21toI420(pSample->getDataPtr(), mSample->getDataPtr(), mSample->mWidth, mSample->mHeight));
    pSample->release();
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

    int pixFmt = -1;
    return_if_fail(getPixelFormat(pSample->getFormat(), pixFmt));

    LOGI("CVideoEncode::onTimer, get one frame and encode, len = %d", pSample->getDataSize());
    int size = mEncoder->encodeVideoFrame((const uint8_t *)pSample->getDataPtr(), 
            (PixelFormat)pixFmt, pSample->mWidth, pSample->mHeight);
    LOGI("CVideoEncode::onTimer, encoded frame len = %d", size);
    if (size > 0) {
        // maybe rtp pack
        write_avc_to_file((char *)mEncoder->getVideoEncodedBuffer(), size);
        if (mEncodeSink) {
            mEncodeSink->onPacked((char *)mEncoder->getVideoEncodedBuffer(), size, PT_RAW);
        }
    }
    pSample->release();
}

