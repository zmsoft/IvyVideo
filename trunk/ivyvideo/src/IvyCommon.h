#ifndef _IVYCOMMON_H_
#define _IVYCOMMON_H_

enum PacketType_t {
    PT_UNKNOWN,
    PT_RTP,     // default v2
    PT_RTCP,    // default v2
    PT_RAW,
};

enum ColorSpace_t {
    CSP_UNKNOWN = -1,
    CSP_I420,
    CSP_NV21,
    CSP_YV12,
    CSP_RGB888, // 24bit
};

enum SizeProfile_t {
    SP_LOW_SIZE,        // 90P: 160x90
    SP_NORMAL_SIZE,     // 180P: 320x180
    SP_HIGH_SIZE,       // 360P: 640x360
};

enum SizeDetail_t{
    LOW_SIZE_WIDTH = 160,
    LOW_SIZE_HEIGHT = 90,
    NORMAL_SIZE_WIDTH = 320,
    NORMAL_SIZE_HEIGHT = 180,
    HIGH_SIZE_WIDTH = 640,
    HIGH_SIZE_HEIGHT = 360,
};

enum FpsProfile_t {
    FP_LOW_FPS,         // 0-6, support <= 90P
    FP_NORMAL_FPS,      // 6-15, <= 180P
    FP_HIGH_FPS,        // 15-30, <= 360P
};

enum CpuProfile_t {
    CP_LOW_CPU,         // support <= 90P
    CP_NORMAL_CPU,      // <= 180P
    CP_HIGH_CPU,        // <= 360P
};

enum BandWidthProfile_t {
    BW_LOW_LINK,
    BW_NORMAL_LINK,
    BW_HIGH_LINK,
};

enum AndroidCSP_t {
    ANDROID_NV21 = 17,
    ANDROID_YV12 = 854321,
};

#ifndef MAX_SIZE
#define MAX_SIZE         260
#endif

#ifndef MAX_RTP_SIZE
#define MAX_RTP_SIZE    1280
#endif

#ifndef MAX_SAMPLE_SIZE
#define MAX_SAMPLE_SIZE        (1280*720*3/2)
#endif

#ifndef return_if_fail
#define return_if_fail(p)       do { if(!(p)) {return;} } while(0)
#endif

#ifndef returnf_if_fail
#define returnf_if_fail(p)      do { if(!(p)) {return false;} } while(0)
#endif

#ifndef returnv_if_fail
#define returnv_if_fail(p, v)   do { if(!(p)) {return (v);} } while(0)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         do {if (p) {delete p; p = NULL;} } while(0)
#endif


typedef struct RawFrameFormat_t {
    int fmt;            // enum ColorSpace
    int width;
    int height;
    int orientation;    // rotation degree
}RawFrameFormat;

#endif // _IVYCOMMON_H_

