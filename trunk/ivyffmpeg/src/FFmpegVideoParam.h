///
/// @file
///
/// @brief  Head file for the video parameters for encoder/decoder
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


#ifndef FFmpegVideoParam_H
#define FFmpegVideoParam_H

#include <string>

extern "C"
{
#include "avformat.h"
}

#ifdef DLL_FILE
#   define FFMPEG_EXPORT _declspec(dllexport)
#else
#   define FFMPEG_EXPORT _declspec(dllimport)
#endif

///
/// @brief  The video parameter class for FFmpegEncoder initializing
///
class FFMPEG_EXPORT FFmpegVideoParam
{
public:
    ///
    /// @brief  Constructor for initializing an object of FFmpegVideoParam
    ///
    ///	@param  [in] width          The width of the video frame, must be greater than 0
    ///	@param  [in] height         The height of the video frame, must be greater than 0
    ///	@param  [in] pixelFormat    PixelFormat enum representing the pixel format of the source video frame
    ///	@param  [in] bitRate        The target bit rate of the target video stream, must be greater than 0
    ///	@param  [in] frameRate      The frame rate of the target video, must be greater than 0
    ///	@param  [in] videoCodecName The name of the video codec which is going to be used in encoding/decoding
    ///
    FFmpegVideoParam(int width, int height, PixelFormat pixelFormat, int bitRate, int frameRate, std::string videoCodecName = "");

    ///
    /// @brief  Constructor for initializing an empty FFmpegVideoParam object
    ///
    FFmpegVideoParam();

    ///
    /// @brief  Destructor
    ///
    virtual ~FFmpegVideoParam();

    ///
    ///	@brief  Judge whether a FFmpegVideoParam object is empty
    ///	
    bool empty();

public:
    int width;                  ///< The width of the video
    int height;                 ///< The height of the video
    PixelFormat pixelFormat;    ///< The pixel format of the video
    int bitRate;                ///< The bit rate of the video
    int frameRate;              ///< The frame rate of the video
    std::string videoCodecName; ///< The name of the video codec
};

#endif//FFmpegVideoParam_H
