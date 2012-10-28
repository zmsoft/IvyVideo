///
/// @file
///
/// @brief  Head file for decoder of FFmpeg
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
///   Changed:  Bug fix, change the video output format to AVPicture
/// - Version:  0.2.1
///   Author:   John (john.zywu@gmail.com)
///   Date:     2008/06/26
///   Changed:  Changed some of the interfaces
///

#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder()
{
    // initialize the private fields
    this->inputContext     = NULL;
    this->videoStream      = NULL;
    this->audioStream      = NULL;
    this->videoFrameBuffer = NULL;
    this->videoFrameSize   = 0;
    this->videoBufferSize  = 0;
    this->audioFrameBuffer = NULL;
    this->audioFrameSize   = 0;
    this->audioBufferSize  = 0;
    this->audioPacketData  = NULL;
    this->audioPacketSize  = 0;
    this->currentPacketPts = 0;
    this->currentPacketDts = 0;
    this->decodeVideo      = false;
    this->decodeAudio      = false;
    this->opened           = false;

    // register all codecs and demux
    av_register_all();
}

FFmpegDecoder::~FFmpegDecoder()
{
    this->close();
}


//////////////////////////////////////////////////////////////////////////
//
//  Public properties
//
//////////////////////////////////////////////////////////////////////////

const FFmpegVideoParam &FFmpegDecoder::getVideoParam() const
{
    return this->videoParam;
}

const FFmpegAudioParam &FFmpegDecoder::getAudioParam() const
{
    return this->audioParam;
}

const uint8_t *FFmpegDecoder::getVideoFrame() const
{
    return this->videoFrameBuffer;
}

int FFmpegDecoder::getVideoFrameSize() const
{
    return this->videoFrameSize;
}

const uint8_t *FFmpegDecoder::getAudioFrame() const
{
    return this->audioFrameBuffer;
}

int FFmpegDecoder::getAudioFrameSize() const
{
    return this->audioFrameSize;
}

double FFmpegDecoder::getPresentTimeStamp() const
{
    return this->currentPacketPts;
}

double FFmpegDecoder::getDecodeTimeStamp() const
{
    return this->currentPacketDts;
}


//////////////////////////////////////////////////////////////////////////
//
//  Public Methods
//
//////////////////////////////////////////////////////////////////////////

void FFmpegDecoder::open(const char *fileName)
{
    if (this->opened)
    {
        throw runtime_error("The decoder is already opened. Call close before opening a new file for decoding.");
    }

    // open a media file as input.
    // The codecs are not opened. Only the file header (if present) is read
    if (av_open_input_file(&this->inputContext, fileName, NULL, 0, NULL))
    {
        throw runtime_error(string("Could not open the media file as input: ") + fileName);
    }

    // Read packets of a media file to get stream information.
    if (av_find_stream_info(this->inputContext) < 0)
    {
        throw runtime_error("Could not get stream information from the media file.");
    }

    // find the video/audio stream
    for (size_t i = 0; i < this->inputContext->nb_streams; i++)
    {
        // TODO
        // there might be several audio or video streams,
        // however, only one audio/video stream is used here
        if (!this->videoStream && this->inputContext->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)
        {
            this->videoStream = this->inputContext->streams[i];
            this->decodeVideo = true;
            continue;
        }

        if (!this->audioStream && this->inputContext->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO)
        {
            this->audioStream = this->inputContext->streams[i];
            this->decodeAudio = true;
            continue;
        }
    }

    // video related initialization if necessary
    if (this->decodeVideo)
    {
        // initialize the video codec
        AVCodecContext *videoCodecContext = this->videoStream->codec;
        AVCodec *videoCodec = avcodec_find_decoder(videoCodecContext->codec_id);
        if (!videoCodec)
        {
            throw runtime_error("Video codec not found.");
        }

        // get the video parameters
        this->videoParam.width       = videoCodecContext->width;
        this->videoParam.height      = videoCodecContext->height;
        this->videoParam.pixelFormat = videoCodecContext->pix_fmt;
        this->videoParam.bitRate     = videoCodecContext->bit_rate;
        this->videoParam.frameRate   = this->videoStream->r_frame_rate.num / this->videoStream->r_frame_rate.den;

        // open the video codec
        if (avcodec_open(videoCodecContext, videoCodec))
        {
            throw runtime_error("Could not open the video codec.");
        }

        // allocate the video frame to be encoded
        this->videoBufferSize  = avpicture_get_size(this->videoParam.pixelFormat, this->videoParam.width, this->videoParam.height);
        this->videoFrameSize   = 0;
        this->videoFrameBuffer = (uint8_t *)av_malloc(this->videoBufferSize);
        if (!this->videoFrameBuffer)
        {
            throw bad_alloc("Could not allocate video frame.");
        }
    }

    // audio related initialization if necessary
    if (this->decodeAudio)
    {
        // initialize the audio codec
        AVCodecContext *audioCodecContext = this->audioStream->codec;
        AVCodec *audioCodec = avcodec_find_decoder(audioCodecContext->codec_id);
        if (!audioCodec)
        {
            throw runtime_error("Audio codec not found.");
        }

        // get the audio parameters
        this->audioParam.sampleRate = audioCodecContext->sample_rate;
        this->audioParam.channels   = audioCodecContext->channels;
        this->audioParam.bitRate    = audioCodecContext->bit_rate;

        // open the audio codec
        if (avcodec_open(audioCodecContext, audioCodec))
        {
            throw runtime_error("Could not open the audio codec.");
        }

        // allocate output buffer
        this->audioBufferSize  = AVCODEC_MAX_AUDIO_FRAME_SIZE;
        this->audioFrameSize   = 0;
        this->audioFrameBuffer = (uint8_t *)av_malloc(this->audioBufferSize);
        this->audioPacketData  = NULL;
        this->audioPacketSize  = 0; // no data in the packet now, for initialization
    }

    this->opened = true;
}


void FFmpegDecoder::close()
{
    if (!this->opened)
    {
        return;
    }

    this->currentPacketPts = 0;
    this->currentPacketDts = 0;

    if (this->decodeVideo)
    {
        avcodec_close(this->videoStream->codec);
        av_freep(&this->videoFrameBuffer);
        this->videoFrameSize = 0;
        this->videoBufferSize = 0;
    }
    if (this->decodeAudio)
    {
        avcodec_close(this->audioStream->codec);
        av_freep(&this->audioFrameBuffer);
        this->audioFrameSize  = 0;
        this->audioBufferSize = 0;
        this->audioPacketData = NULL;
        this->audioPacketSize = 0;
    }

    // close the input file
    av_close_input_file(this->inputContext);

    this->inputContext = NULL;
    this->audioStream  = NULL;
    this->videoStream  = NULL;
    this->currentPacket = AVPacket();

    this->audioParam  = FFmpegAudioParam();
    this->videoParam  = FFmpegVideoParam();

    this->opened      = false;
    this->decodeAudio = false;
    this->decodeVideo = false;
}

int FFmpegDecoder::decodeFrame()
{
    if (!this->opened)
    {
        throw runtime_error("Please call open() before decoding and reading frame.");
    }

    // one audio packet may contains several audio frame, so we need to
    // make sure all the audio frames in current packet were decoded.
    if (this->audioPacketSize <= 0)
    {
        // all data in the audio packet have been decoded,
        // read new frame now
        if (this->readFrame() < 0)
        {
            return -1;
        }
    }

    if (this->videoStream && this->currentPacket.stream_index == this->videoStream->index)
    {
        // decode video frame
        this->currentPacketPts = (double)this->currentPacket.pts * this->videoStream->time_base.num / this->videoStream->time_base.den;
        this->currentPacketDts = (double)this->currentPacket.dts * this->videoStream->time_base.num / this->videoStream->time_base.den;
        this->decodeVideoFrame();
        return 0;
    }
    else if (this->audioStream && this->currentPacket.stream_index == this->audioStream->index)
    {
        // decode audio frame
        this->currentPacketPts = (double)this->currentPacket.pts * this->audioStream->time_base.num / this->audioStream->time_base.den;
        this->currentPacketDts = (double)this->currentPacket.dts * this->audioStream->time_base.num / this->audioStream->time_base.den;
        this->decodeAudioFrame();
        return 1;
    }
    else
    {
        // error
        return -1;
    }
}


//////////////////////////////////////////////////////////////////////////
//
//  Private Methods
//
//////////////////////////////////////////////////////////////////////////

int FFmpegDecoder::readFrame()
{
    av_init_packet(&this->currentPacket);

    // read frame from input
    if (av_read_frame(this->inputContext, &this->currentPacket) < 0)
    {
        return -1;
    }

    // For video, the returned packet contain exactly one frame.
    // For audio, it contains an integer number of frames if each frame has
    // a known fixed size (e.g. PCM or ADPCM) data.
    // It contains only one frame if the audio frames have a variable size (e.g. MPEG audio).

    if (this->currentPacket.stream_index == this->audioStream->index)
    {
        // set the audio packet data to be decoded
        // for dealing with one packet contains several audio frames
        this->audioPacketData = this->currentPacket.data;
        this->audioPacketSize = this->currentPacket.size;
    }

    return 0;
}

void FFmpegDecoder::decodeVideoFrame()
{
    int decodedSize, gotPicture = 0;
    AVFrame videoFrame;

    // set default value
    avcodec_get_frame_defaults(&videoFrame);

    // decode the video frame
    decodedSize = avcodec_decode_video(this->videoStream->codec, &videoFrame, &gotPicture, this->currentPacket.data, this->currentPacket.size);

    this->videoFrameSize = 0;
    if (gotPicture != 0)
    {
        // read the data to the buffer
        avpicture_layout((AVPicture*)&videoFrame, this->videoParam.pixelFormat, this->videoParam.width, this->videoParam.height, this->videoFrameBuffer, this->videoBufferSize);
        this->videoFrameSize = this->videoBufferSize;
    }

    // video frame is decoded
    // free the packet now
    av_free_packet(&this->currentPacket);

    if (decodedSize < 0)
    {
        throw runtime_error("Fail to decode a video frame.");
    }
}

void FFmpegDecoder::decodeAudioFrame()
{
    int decodedSize, outputFrameSize = this->audioBufferSize;

    // decode one audio frame
    decodedSize = avcodec_decode_audio2(this->audioStream->codec, (int16_t *)this->audioFrameBuffer, &outputFrameSize, this->audioPacketData, this->audioPacketSize);

    this->audioFrameSize = outputFrameSize;

    if (this->audioPacketSize - decodedSize <= 0)
    {
        // all the audio frames in the packet have been decoded
        // free the packet now
        av_free_packet(&this->currentPacket);
    }

    if (decodedSize< 0)
    {
        throw runtime_error("Fail to decode a audio frame.");
    }
    else
    {
        this->audioPacketData += decodedSize;
        this->audioPacketSize -= decodedSize;
    }
}
