#ifndef _IVYCLIENT_H_
#define _IVYCLIENT_H_

#include "IvyCommon.h"
#include "IvyVideo.h"
#include "MutexLock.h"

class CIvyClient : 
    public IvyVideoEncodeSink,
    public IvyVideoDecodeSink
{
protected:
    CIvyClient();
    ~CIvyClient();

public:
    static CIvyClient *inst();

    bool init();
    void uninit();

    // for video encoder
    bool startSelfVideo();
    bool stopSelfVideo();
    void setVideoEncodeParams(const char *codec);
    void onRawFrame(char *data, int len, RawFrameFormat format);
    void setExternalVideoEncodeSink(IvyVideoEncodeSink *sink);

    // for video decoder
    bool requestPeerVideo();
    bool cancelPeerVideo();

    // for IvyVideoEncodeSink
    virtual void onPacked(char *data, int size, int type);

    // for IvyVideoDecodeSink
    virtual void onDecoded(char *data, int size, int format);

private:
    IvyVideoEncode *mVideoEncode;
    IvyVideoDecode *mVideoDecode;
    std::string     mVideoEncodecName;
    IvyVideoEncodeSink *mVideoEncodeSink;

    CMutex mEncodeMutex;
    CMutex mDecodeMutex;

    static CIvyClient *mClient;
};

#endif // _IVYCLIENT_H_

