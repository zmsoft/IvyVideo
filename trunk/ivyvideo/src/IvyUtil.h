#ifndef _IVYUTIL_H_
#define _IVYUTIL_H_

bool getSizeDetail(int size, int &width, int &height);
bool getPixelFormat(int fmt, int &pixFmt);

bool NV21toI420(const char *src, char *dst, int width, int height);
bool YV12toI420(const char *src, char *dst, int width, int height);

bool CropYUVFrame(const char *src, unsigned int src_w, unsigned int src_h, int format,
        char *dst, unsigned int dst_w, unsigned int dst_h);

#endif

