#ifndef _IVYCLIENT_H_
#define _IVYCLIENT_H_

#include "IvyCommon.h"
#include "IvyVideo.h"

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
    void onRawFrame(char *data, int len, RawFrameFormat format);

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

    static CIvyClient *mClient;
};

#endif // _IVYCLIENT_H_

