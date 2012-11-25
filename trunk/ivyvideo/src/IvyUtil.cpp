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

bool NV21toI420(const char *src, char *dst, int width, int height)
{
    if (!src || !dst) {
        return false;
    }

    unsigned int YSize = width * height;
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

bool YV12toI420(const char *src, char *dst, int width, int height)
{
    if (!src || !dst) {
        return false;
    }

    unsigned int YSize = width * height;
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

bool CropYUVFrame(const char *src, unsigned int src_w, unsigned int src_h, int format,
        char *dst, unsigned int dst_w, unsigned int dst_h)
{
    if (!src || !dst) {
        return false;
    }

    if (src_w < dst_w || src_h < dst_h) 
    {
        return false;
    }

    if (src_w == dst_w && src_h == dst_h) 
    {
        return true;
    }

    unsigned int crop_x = (src_w - dst_w)>>1;
    unsigned int crop_y = (src_h - dst_h)>>1;

    if (format == CSP_I420) {
        // for src
        unsigned int src_ysize = src_w * src_h;
        unsigned int src_usize = (src_ysize>>2);
        unsigned int src_vsize = (src_ysize>>2);

        unsigned int src_ylinesize = src_w;
        unsigned int src_ulinesize = (src_ylinesize>>2);
        unsigned int src_vlinesize = (src_ylinesize>>2);

        // for dst
        unsigned int dst_ysize = dst_w * dst_h;
        unsigned int dst_usize = (dst_ysize>>2);
        unsigned int dst_vsize = (dst_ysize>>2);

        unsigned int dst_ylinesize = dst_w;
        unsigned int dst_ulinesize = (dst_ylinesize>>2);
        unsigned int dst_vlinesize = (dst_ylinesize>>2);

        // copy Y
        const char *src_y = src + (crop_y>>1)*src_ylinesize + (crop_x>>1);
        char *dst_y = dst;
        for (int k=0; k < dst_h; k++) {
            memcpy(dst_y, src_y, dst_ylinesize);
            src_y += src_ylinesize;
            dst_y += dst_ylinesize;
        }

        // copy U
        const char *src_u = (src + src_ysize) + (crop_y>>1) * src_ulinesize + (crop_x>>1);
        char *dst_u = dst + dst_ysize;
        for (int k=0; k < dst_h; k++) {
            memcpy(dst_u, src_u, dst_ulinesize);
            src_u += src_ulinesize;
            dst_u += dst_ulinesize;
        }

        // copy V
        const char *src_v = (src + src_ysize + src_usize) + (crop_y>>1) * src_vlinesize + (crop_x>>1);
        char *dst_v = dst + dst_ysize + dst_usize;
        for (int k=0; k < dst_h; k++) {
            memcpy(dst_v, src_v, dst_vlinesize);
            src_v += src_vlinesize;
            dst_v += dst_vlinesize;
        }
    }
    else if (format == CSP_NV21)
    {
        // for src
        unsigned int src_ysize = src_w * src_h;
        unsigned int src_uvsize = (src_ysize>>1);

        unsigned int src_ylinesize = src_w;
        unsigned int src_uvlinesize = (src_ylinesize>>1);

        // for dst
        unsigned int dst_ysize = dst_w * dst_h;
        unsigned int dst_uvsize = (dst_ysize>>1);

        unsigned int dst_ylinesize = dst_w;
        unsigned int dst_uvlinesize = (dst_ylinesize>>1);

        // copy Y
        const char *src_y = src + (crop_y>>1)*src_ylinesize + (crop_x>>1);
        char *dst_y = dst;
        for (int k=0; k < dst_h; k++) {
            memcpy(dst_y, src_y, dst_ylinesize);
            src_y += src_ylinesize;
            dst_y += dst_ylinesize;
        }

        // copy U and V
        const char *src_uv = (src + src_ysize) + (crop_y>>1) * src_uvlinesize + crop_x;
        char *dst_uv = dst + dst_ysize;
        for (int k=0; k < dst_h; k++) {
            memcpy(dst_uv, src_uv, dst_uvlinesize);
            src_uv += src_uvlinesize;
            dst_uv += dst_uvlinesize;
        }
    }
    else
    {
        return false;
    }

    return true;
}

