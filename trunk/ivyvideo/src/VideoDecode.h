#ifndef _VIDEODECODE_H_
#define _VIDEODECODE_H_

#include "IvyVideo.h"
#include "IDummyObject.h"

class CVideoDecode : public IDummyObject
{
public:
	CVideoDecode();
	virtual ~CVideoDecode();

	bool init();
	void uninit();

	void onPacketData(char *data, int size);

private:
	IvyVideoDecodeSink *mDecodeSink;
};


#endif // _VIDEODECODE_H_

