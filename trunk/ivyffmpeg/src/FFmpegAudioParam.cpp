///
/// @file
///
/// @brief  Implementation file for the audio parameters for encoder/decoder
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

#include "FFmpegAudioParam.h"

FFmpegAudioParam::FFmpegAudioParam(int sampleRate, int channels, int bitRate, std::string audioCodecName) : 
    sampleRate(sampleRate), channels(channels), bitRate(bitRate), audioCodecName(audioCodecName)
{
    // valid the arguments
    if (bitRate < 1 || sampleRate < 1 || channels < 1)
    {
        throw std::invalid_argument("The arguments for the constructor of FFmpegAudioParam is invalid.");
    }
}

FFmpegAudioParam::FFmpegAudioParam() :
    sampleRate(0), channels(0), bitRate(0), audioCodecName("")
{
}

FFmpegAudioParam::~FFmpegAudioParam()
{
}

bool FFmpegAudioParam::empty()
{
    return bitRate < 1 && sampleRate < 1 && channels < 1 && audioCodecName == "";
}
