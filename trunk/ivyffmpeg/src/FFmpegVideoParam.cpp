///
/// @file
///
/// @brief  Implementation file for the video parameters for encoder/decoder
///
/// @version    0.2.1
/// @date       2008/06/26
///
/// <b>History:</b>
/// - Version:  0.1.0
///   Author:   farthinker (farthinker@gmail.com)
///   Date:     2008/05/14
///   Changed:  Created
/// - Version:  0.2.0
///   Author:   farthinker (farthinker@gmail.com)
///   Date:     2008/06/06
///   Changed:  Bug fix
/// - Version:  0.2.1
///   Author:   John (john.zywu@gmail.com)
///   Date:     2008/06/26
///   Changed:  Changed some of the interfaces
///

#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "FFmpegVideoParam.h"


FFmpegVideoParam::FFmpegVideoParam(int width, int height, PixelFormat pixelFormat, int bitRate, int frameRate, std::string videoCodecName) :
    width(width), height(height), pixelFormat(pixelFormat), bitRate(bitRate), frameRate(frameRate), videoCodecName(videoCodecName)
{
    // valid the arguments
    if (width < 1 || height < 1 || pixelFormat == PIX_FMT_NONE || bitRate < 1 || frameRate < 1)
    {
        throw std::invalid_argument("The arguments for the constructor of FFmpegVideoParam is invalid.");
    }
}

FFmpegVideoParam::FFmpegVideoParam() :
    width(0), height(0), pixelFormat(PIX_FMT_NONE), bitRate(0), frameRate(0), videoCodecName("")
{
}

FFmpegVideoParam::~FFmpegVideoParam()
{
}

bool FFmpegVideoParam::empty()
{
    return width < 1 && height < 1 && pixelFormat == PIX_FMT_NONE  && 
        bitRate < 1 && frameRate < 1 && videoCodecName == "";
}
