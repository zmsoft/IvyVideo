#ifndef _IVYVIDEO_H_
#define _IVYVIDEO_H_

class IvyVideoEncodeSink
{
public:
	// now only output RTP, next maybe support RTCP
	virtual void onPacked(char *data, int size, int type) = 0;
};

class IvyVideoDeocdeSink
{
public:
	// now only output I420 data
	virtual void onDecoded(char *data, int size, int format) = 0;
};

#endif

