#include "IvyClient.h"
#include "VideoEncode.h"
#include "IvyUtil.h"

CIvyClient *CIvyClient::mClient = NULL;

CIvyClient *CIvyClient::inst() 
{
    if (mClient == NULL) {
        mClient = new CIvyClient;
    }

    return mClient;
}

CIvyClient::CIvyClient()
{
    mVideoEncode = NULL;
    mVideoDecode = NULL;
}

CIvyClient::~CIvyClient()
{
    if (mVideoEncode) {
        delete mVideoEncode;
    }

    if (mVideoDecode) {
        delete mVideoDecode;
    }
}

bool CIvyClient::init()
{
    return true;
}

void CIvyClient::uninit()
{}


//
// for video encoder
//
bool CIvyClient::startSelfVideo()
{
    CAutoLock lock(mEncodeMutex);
    if (mVideoEncode == NULL) {
        mVideoEncode = new CVideoEncode(this);
    }

    //TODO: query capture capability and host performance to
    // determine the parameters of encoder: fps, resolution, cpu
    int size = SP_LOW_SIZE;
    int width = 0;
    int height = 0;
    getSizeDetail(size, width, height);

    int fmt = CSP_I420;
    int bandwidth = 512*1024; // kb
    int fps = 25;
    return_val_if_fail(mVideoEncode->init(width, height, fmt, 
                fps, bandwidth), false);

    return true;
}

bool CIvyClient::stopSelfVideo()
{
    CAutoLock lock(mEncodeMutex);
    return_val_if_fail(mVideoEncode != NULL, true);
    mVideoEncode->uninit();

    return true;
}

void CIvyClient::onRawFrame(char *data, int len, RawFrameFormat format)
{
    CAutoLock lock(mEncodeMutex);
    if (mVideoEncode != NULL) {
        mVideoEncode->onRawFrame(data, len, format);
    }
}

//
// for video decoder
//
bool CIvyClient::requestPeerVideo()
{
    return false;
}

bool CIvyClient::cancelPeerVideo()
{
    return false;
}

void CIvyClient::onPacked(char *data, int size, int type)
{
}

void CIvyClient::onDecoded(char *data, int size, int format)
{
}

