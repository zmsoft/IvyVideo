#include "FFmpegVideoParam.h"

FFmpegVideoParam::FFmpegVideoParam(int width, int height, PixelFormat pixelFormat, int bitRate, int frameRate, std::string videoCodecName) :
    width(width), height(height), pixelFormat(pixelFormat), bitRate(bitRate), frameRate(frameRate), videoCodecName(videoCodecName)
{
}

FFmpegVideoParam::FFmpegVideoParam() :
    width(0), height(0), pixelFormat(PIX_FMT_NONE), bitRate(0), frameRate(0), videoCodecName("")
{
}

FFmpegVideoParam::~FFmpegVideoParam()
{
}

bool FFmpegVideoParam::isValid()
{
    // valid the arguments
    if (width < 1 || height < 1 || pixelFormat == PIX_FMT_NONE || bitRate < 1 || frameRate < 1)
    {
        return false;
    }

    return true;
}

bool FFmpegVideoParam::empty()
{
    return width < 1 && height < 1 && pixelFormat == PIX_FMT_NONE  && 
        bitRate < 1 && frameRate < 1 && videoCodecName == "";
}
