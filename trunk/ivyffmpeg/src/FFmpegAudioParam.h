///
/// @file
///
/// @brief  Head file for the audio parameters for encoder/decoder
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

#ifndef FFmpegAudioParam_H
#define FFmpegAudioParam_H

#include <string>

#ifdef DLL_FILE
#   define FFMPEG_EXPORT _declspec(dllexport)
#else
#   define FFMPEG_EXPORT _declspec(dllimport)
#endif

///
/// @brief  The audio parameter class for FFmpegEncoder initializing
///
class FFMPEG_EXPORT FFmpegAudioParam
{
public:
    ///
    /// @brief  Constructor for initializing an object of FFmpegAudioParam
    ///
    /// @param  [in] sampleRate     The sample rate of the audio, must be greater than 0
    /// @param  [in] channels       The number of channels in the audio, must be greater than 0
    /// @param  [in] bitRate        The target bit rate of the target audio stream, must be greater than 0
    /// @param  [in] audioCodecName The name of the audio codec which is going to be used in encoding/decoding
    ///
    FFmpegAudioParam(int sampleRate, int channels, int bitRate, std::string audioCodecName = "");

    ///
    /// @brief  Constructor for initializing an empty FFmpegAudioParam object
    ///
    FFmpegAudioParam();

    ///
    /// @brief  Destructor
    ///
    virtual ~FFmpegAudioParam();

    ///
    ///	@brief  Judge whether a FFmpegAudioParam object is empty
    ///	
    bool empty();

public:
    int sampleRate;             ///< The sample rate of the audio
    int channels;               ///< The number of audio channels
    int bitRate;                ///< The bit rate of the audio
    std::string audioCodecName;	///< The name of the audio codec
};

#endif//FFmpegAudioParam_H
