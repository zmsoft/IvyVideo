#ifndef _VIDEODECODE_H_
#define _VIDEODECODE_H_

#include "IvyCommon.h"
#include "IvyVideo.h"
#include "IDummyObject.h"

class CVideoDecode : 
    public IvyVideoDecode,
    public IDummyObject
{
public:
    CVideoDecode();
    virtual ~CVideoDecode();

    virtual bool init();
    virtual void uninit();
    virtual void onPacketData(char *data, int size);

private:
    IvyVideoDecodeSink *mDecodeSink;
};


#endif // _VIDEODECODE_H_

