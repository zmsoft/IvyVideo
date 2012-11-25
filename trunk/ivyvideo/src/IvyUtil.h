#ifndef _IVYUTIL_H_
#define _IVYUTIL_H_

bool getSizeDetail(int size, int &width, int &height);
bool getPixelFormat(int fmt, int &pixFmt);

bool NV21toI420(const char *src, char *dst, int linesize, int height);
bool YV12toI420(const char *src, char *dst, int linesize, int height);

#endif

