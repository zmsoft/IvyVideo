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

bool NV21toI420(const char *src, char *dst, int linesize, int height)
{
    if (!src || !dst) {
        return false;
    }

    unsigned int YSize = linesize * height;
    unsigned int UVSize = (YSize>>1);

    // NV21: Y..Y + VUV...U
    const char *pSrcY = src;
    const char *pSrcUV = src + YSize;

    // I420: Y..Y + U.U + V.V
    char *pDstY = dst;
    char *pDstU = dst + YSize;
    char *pDstV = dst + YSize + (UVSize>>1);
    
    // copy Y
    memcpy(pDstY, pSrcY, YSize);

    // copy U and V
    for (int k=0; k < (UVSize>>1); k++) {
        pDstV[k] = pSrcUV[k*2];     // copy V
        pDstU[k] = pSrcUV[k*2+1];   // copy U
    }

    return true;
}

bool YV12toI420(const char *src, char *dst, int linesize, int height)
{
    if (!src || !dst) {
        return false;
    }

    unsigned int YSize = linesize * height;
    unsigned int UVSize = (YSize>>1);

    const char *pSrcY = src;
    const char *pSrcV = src + YSize;
    const char *pSrcU = src + YSize + (UVSize>>1);

    char *pDstY = dst;
    char *pDstU = dst + YSize;
    char *pDstV = dst + YSize + (UVSize>>1);
    
    memcpy(pDstY, pSrcY, YSize);
    memcpy(pDstU, pSrcU, (UVSize>>1));
    memcpy(pDstV, pSrcV, (UVSize>>1));

    return true;
}

