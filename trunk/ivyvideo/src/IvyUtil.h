#ifndef _IVYUTIL_H_
#define _IVYUTIL_H_

bool getSizeDetail(int size, int &width, int &height);
bool getPixelFormat(int fmt, int &pixFmt);

void NV21toI420(const char *src, char *dst, int width, int height, int pixel);
void YV12toI420(const char *src, char *dst, int width, int height, int pixel);

#endif

