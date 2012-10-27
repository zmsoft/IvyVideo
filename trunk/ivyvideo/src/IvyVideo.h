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

enum PacketType {
	PT_UNKNOWN,
	PT_RTP,		// default v2
	PT_RTCP,	// default v2
};

enum ColorSpace {
	CSP_UNKNOWN = -1,
	CSP_I420,
	CSP_NV21,
	CSP_YV12,
	CSP_RGB888, // 24bit
};

enum SizeProfile {
	EF_LOW_SIZE,		// 90P: 160x90
	EF_NORMAL_SIZE,		// 180P: 320x180
	EF_HIGH_SIZE,		// 360P: 640x360
};

enum FpsProfile {
	FP_LOW_FPS,			// 0-6, support <= 90P
	FP_NORMAL_FPS,		// 6-15, <= 180P
	FP_HIGH_FPS,		// 15-30, <= 360P
};

enum CpuProfile {
	CP_LOW_CPU,			// support <= 90P
	CP_NORMAL_CPU,		// <= 180P
	CP_HIGH_CPU,		// <= 360P
};

enum BandWidthProfile {
	BW_LOW_LINK,
	BW_NORMAL_LINK,
	BW_HIGH_LINK,
};

#ifndef MAX_SIZE
#define MAX_SIZE 		260
#endif

#ifndef MAX_RTP_SIZE
#define MAX_RTP_SIZE	1280
#endif


#endif // _IVYVIDEO_H_

