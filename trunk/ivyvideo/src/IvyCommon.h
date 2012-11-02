#ifndef _IVYCOMMON_H_
#define _IVYCOMMON_H_

enum PacketType {
	PT_UNKNOWN,
	PT_RTP,		// default v2
	PT_RTCP,	// default v2
	PT_RAW,
};

enum ColorSpace {
	CSP_UNKNOWN = -1,
	CSP_I420,
	CSP_NV21,
	CSP_YV12,
	CSP_RGB888, // 24bit
};

enum SizeProfile {
	SP_LOW_SIZE,		// 90P: 160x90
	SP_NORMAL_SIZE,		// 180P: 320x180
	SP_HIGH_SIZE,		// 360P: 640x360
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

#ifndef MAX_SAMPLE_SIZE
#define MAX_SAMPLE_SIZE		(1280*720*3/2)
#endif

#ifndef return_if_fail
#define return_if_fail(p)	do { if(!(p)) {return;} } while(0)
#endif

#ifndef return_val_if_fail
#define return_val_if_fail(p, v)	do { if(!(p)) {return (v);} } while(0)
#endif

typedef struct RawFrameFormat_t {
	int fmt;		// enum ColorSpace
	int width;
	int height;
	int orientation;	// rotation degree
}RawFrameFormat;

#endif // _IVYVIDEO_H_

