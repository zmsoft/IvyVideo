#ifndef _IVYCLIENT_H_
#define _IVYCLIENT_H_

#include "IvyCommon.h"
#include "IvyVideo.h"

class CIvyClient : 
    public IvyVideoEncodeSink,
    public IvyVideoDecodeSink
{
public:
    CIvyClient();
    ~CIvyClient();

    bool init();
    void uninit();

    // for video encoder and send
    bool startSelfVideo();
    bool stopSelfVideo();

    // for video recv and decoder
    bool requestPeerVideo();
    bool cancelPeerVideo();

    // for IvyVideoEncodeSink
    virtual void onPacked(char *data, int size, int type);

    // for IvyVideoDecodeSink
    virtual void onDecoded(char *data, int size, int format);

private:
    IvyVideoEncode *mVideoEncode;
    IvyVideoDecode *mVideoDecode;
};

#endif // _IVYCLIENT_H_

