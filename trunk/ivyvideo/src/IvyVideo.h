#ifndef _IVYVIDEO_H_
#define _IVYVIDEO_H_

class IvyVideoEncode
{
public:
	virtual bool init() = 0;
	virtual void uninit() = 0;
	virtual void onRawFrame(char *data, int size, int width, int height, int format) = 0;
};

class IvyVideoDecode
{
public:
	virtual bool init() = 0;
	virtual void uninit() = 0;
	virtual void onPacketData(char *data, int size) = 0;
};

class IvyVideoEncodeSink
{
public:
	// now only output RTP, next maybe support RTCP
	virtual void onPacked(char *data, int size, int type) = 0;
};

class IvyVideoDecodeSink
{
public:
	// now only output I420 data
	virtual void onDecoded(char *data, int size, int format) = 0;
};

#endif

