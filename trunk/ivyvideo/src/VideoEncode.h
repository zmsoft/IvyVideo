#ifndef _VIDEOENCODE_H_
#define _VIDEOENCODE_H_

#include "IvyVideo.h"

class CVideoEncode : public IDummyObject
{
public:
	CVideoEncode();
	virtual ~CVideoEncode();

	bool init();
	void uninit();

	void onRawFrame(char *data, int size, int width, int height, int format);

private:
	IVideoEncodeSink *mEncodeSink;
};


#endif // _VIDEOENCODE_H_

