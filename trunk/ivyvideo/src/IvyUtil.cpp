#include "IvyUtil.h"
#include "IvyCommon.h"
#include "libavformat/avformat.h"

bool getSizeDetail(int size, int &width, int &height)
{
    width = LOW_SIZE_WIDTH;
    height = LOW_SIZE_HEIGHT;
    switch(size) {
        case SP_LOW_SIZE:
            width = LOW_SIZE_WIDTH;
            height = LOW_SIZE_HEIGHT;
            break;
        case SP_NORMAL_SIZE:
            width = NORMAL_SIZE_WIDTH;
            height = NORMAL_SIZE_HEIGHT;
            break;
        case SP_HIGH_SIZE:
            width = HIGH_SIZE_WIDTH;
            height = HIGH_SIZE_HEIGHT;
            break;
        default:
            return false;
    }

    return true;
}

bool getPixelFormat(int fmt, int &pixFmt)
{
    switch(fmt) {
        case CSP_I420:
            pixFmt = PIX_FMT_YUV420P;
            break;
        default:
            return false;
    }

    return true;
}

void NV21toI420(const char *src, char *dst, int width, int height, int pixel)
{
    if (!src || !dst) {
        return;
    }

    int YSize = width * height * pixel;
    int UVSize = YSize / 2;

    const char *pSrcY = src;
    const char *pSrcUV = src + YSize;

    char *pDstY = dst;
    char *pDstUV = dst + YSize;
    
    // copy Y
    memcpy(pDstY, pSrcY, YSize);

    // copy U and V
    for (int k=0; k < UVSize; k+=2) {
        pDstUV[k] = pSrcUV[k];
        pDstUV[UVSize/2 + k] = pSrcUV[k+1];
    }
}

void YV12toI420(const char *src, char *dst, int width, int height, int pixel)
{
    if (!src || !dst) {
        return;
    }

    int YSize = width * height * pixel / 8;
    int UVSize = YSize / 2;

    const char *pSrcY = src;
    const char *pSrcUV = src + YSize;

    char *pDstY = dst;
    char *pDstUV = dst + YSize;
    
    memcpy(pDstY, pSrcY, YSize);
    memcpy(pDstY+YSize, pSrcY+YSize+UVSize/2, UVSize/2);
    memcpy(pDstY+YSize+UVSize/2, pSrcY+YSize, UVSize/2);
}

