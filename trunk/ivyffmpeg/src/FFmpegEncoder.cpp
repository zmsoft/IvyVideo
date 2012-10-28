///
/// @file
///
/// @brief  Implementation file for encoder of FFmpeg
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
///   Changed:  Bug fix, change the video input format to AVPicture, 
///             add pixel format convertor
/// - Version:  0.2.1
///   Author:   John (john.zywu@gmail.com)
///   Date:     2008/06/26
///   Changed:  Fixed the memory leak bug, changed some of the interfaces
///


#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "FFmpegEncoder.h"
#include <string>
using namespace std;

// define the max audio packet size as 128 KB
#define MAX_AUDIO_PACKET_SIZE (128 * 1024)


FFmpegEncoder::FFmpegEncoder(const FFmpegVideoParam &videoParam, const FFmpegAudioParam &audioParam) : videoParam(videoParam), audioParam(audioParam)
{
    // initialize the private fields
    this->outputContext = NULL;
    this->videoStream = NULL;
    this->audioStream = NULL;
    this->videoFrame = NULL;
    this->videoBuffer = NULL;
    this->videoBufferSize = 0;
    this->audioBuffer = NULL;
    this->audioBufferSize = 0;
    this->opened = false;
    this->hasOutput = false;
    this->encodeVideo = !this->videoParam.empty();
    this->encodeAudio = !this->audioParam.empty();

    // initialize libavcodec, and register all codecs and formats
    av_register_all();
}

FFmpegEncoder::~FFmpegEncoder()
{
    this->close();
}


//////////////////////////////////////////////////////////////////////////
//
//  Public properties
//
//////////////////////////////////////////////////////////////////////////

const uint8_t *FFmpegEncoder::getVideoEncodedBuffer() const
{
    return this->videoBuffer;
}

double FFmpegEncoder::getVideoTimeStamp() const
{
    if (!this->opened || !this->encodeVideo || !this->hasOutput)
    {
        return 0;
    }
    return (double)this->videoStream->pts.val * this->videoStream->time_base.num / this->videoStream->time_base.den;
}

const FFmpegVideoParam &FFmpegEncoder::getVideoParam() const
{
    return this->videoParam;
}

int FFmpegEncoder::getVideoFrameSize() const
{
    if (!this->opened || !this->encodeVideo)
    {
        return 0;
    }
    return avpicture_get_size(this->videoParam.pixelFormat, this->videoParam.width, this->videoParam.height);
}

const uint8_t *FFmpegEncoder::getAudioEncodedBuffer() const
{
    return this->audioBuffer;
}

double FFmpegEncoder::getAudioTimeStamp() const
{
    if (!this->opened || !this->encodeAudio || !this->hasOutput)
    {
        return 0;
    }
    return (double)this->audioStream->pts.val * this->audioStream->time_base.num / this->audioStream->time_base.den;
}

const FFmpegAudioParam &FFmpegEncoder::getAudioParam() const
{
    return this->audioParam;
}

int FFmpegEncoder::getAudioFrameSize() const
{
    if (!this->opened || !this->encodeAudio)
    {
        return 0;
    }

    int frameSize = 0;
    if (this->audioStream->codec && this->audioStream->codec->frame_size > 1)
    {
        frameSize  = this->audioStream->codec->frame_size;
        frameSize *= this->audioStream->codec->channels;    // multiply the channels
        frameSize *= sizeof(short); // convert to bytes
    }
    else
    {
        // hack for PCM audio codec
        //frameSize = this->audioBufferSize / this->audioParam.channels;
        //switch (this->audioStream->codec->codec_id)
        //{
        //    case CODEC_ID_PCM_S16LE:
        //    case CODEC_ID_PCM_S16BE:
        //    case CODEC_ID_PCM_U16LE:
        //    case CODEC_ID_PCM_U16BE:
        //        frameSize >>= 1;
        //        break;
        //    default:
        //        break;
        //}
        frameSize = this->audioBufferSize;  // including all channels, return bytes directly
    }
    return frameSize;
}


//////////////////////////////////////////////////////////////////////////
//
//  Public Methods
//
//////////////////////////////////////////////////////////////////////////

int FFmpegEncoder::encodeVideoFrame(const uint8_t *frameData)
{
    if (!this->opened)
    {
        throw runtime_error("Please call open() before encoding.");
    }

    if (!this->encodeVideo)
    {
        throw runtime_error("Can not encode video frame because video codec is not initialized.");
    }

    if (this->hasOutput)
    {
        throw runtime_error("Please use writeVideoFrame() if there is output.");
    }

    // encode the image frame
    AVPicture picture;
    avpicture_fill(&picture, (uint8_t *)frameData, this->videoParam.pixelFormat, this->videoParam.width, this->videoParam.height);
    return this->encodeVideoData(&picture);
}

int FFmpegEncoder::writeVideoFrame(const uint8_t *frameData)
{
    if (!this->opened)
    {
        throw runtime_error("Please call open() before encoding.");
    }

    if (!this->encodeVideo)
    {
        throw runtime_error("Can not encode video frame because video codec is not initialized.");
    }

    if (!this->hasOutput)
    {
        throw runtime_error("Please use encodeVideoFrame() if there is no output.");
    }

    // encode the image
    AVPicture picture;
    avpicture_fill(&picture, (uint8_t *)frameData, this->videoParam.pixelFormat, this->videoParam.width, this->videoParam.height);
    int encodedSize = this->encodeVideoData(&picture);

    // output the encoded image data
    if (encodedSize > 0)
    {
        this->writeVideoData(this->videoBuffer, encodedSize);
    }
    return encodedSize;
}

int FFmpegEncoder::encodeAudioFrame(const uint8_t *frameData, int dataSize)
{
    if (!this->opened)
    {
        throw runtime_error("Please call open() before encoding.");
    }

    if (!this->encodeAudio)
    {
        throw runtime_error("Can not encode video frame because audio codec is not initialized.");
    }

    if (this->hasOutput)
    {
        throw runtime_error("Please use writeAudioFrame() if there is output.");
    }

    if (this->audioStream->codec->frame_size <= 1 && dataSize < 1)
    {
        throw runtime_error("Parameter dataSize must be set for some specific (e.g. PCM) codecs.");
    }

    return this->encodeAudioData((short*)frameData, dataSize/sizeof(short));
}

int FFmpegEncoder::writeAudioFrame(const uint8_t *frameData, int dataSize)
{
    if (!this->opened)
    {
        throw runtime_error("Please call open() before encoding.");
    }

    if (!this->encodeAudio)
    {
        throw runtime_error("Can not encode video frame because audio codec is not initialized.");
    }

    if (!this->hasOutput)
    {
        throw runtime_error("Please use encodeVideoFrame() if there is no output.");
    }

    if (this->audioStream->codec->frame_size <= 1 && dataSize < 1)
    {
        throw runtime_error("Parameter dataSize must be set for some specific (e.g. PCM) codecs.");
    }

    // encode the audio data
    int encodedSize = this->encodeAudioData((short*)frameData, dataSize/sizeof(short));

    // output the encoded audio data
    if (encodedSize > 0)
    {
        this->writeAudioData(this->audioBuffer, encodedSize);
    }
    return encodedSize;
}

void FFmpegEncoder::open(const char *fileName)
{
    if (this->opened)
    {
        throw runtime_error("The encoder is already opened. Call close before opening a new encoder.");
    }

    this->hasOutput = (fileName != NULL) && (fileName[0] != 0);
    if (!this->hasOutput && this->videoParam.videoCodecName.empty() && this->audioParam.audioCodecName.empty())
    {
        throw invalid_argument("The encoder must have output file or video/audio codec set.");
    }

    // initialize the output format
    AVOutputFormat *outputFormat = NULL;
    if (this->hasOutput)
    {
        // find suitable output format according to the postfix of the filename
        outputFormat = guess_format(NULL, fileName, NULL);
        if(!outputFormat)
        {
            throw runtime_error("Couldn't find corresponding format for the output file.");
        }
    }

    // allocate the output media context
    this->outputContext = av_alloc_format_context();
    if (!this->outputContext)
    {
        throw bad_alloc("Couldn't allocate the output media context.");
    }
    if (this->hasOutput)
    {
        this->outputContext->oformat = outputFormat;
        _snprintf(this->outputContext->filename, sizeof(this->outputContext->filename), "%s", fileName);
    }

    // video related initialization if necessary
    if (this->encodeVideo)
    {
        // validate the video codec
        if ((!outputFormat || outputFormat->video_codec == CODEC_ID_NONE) && this->videoParam.videoCodecName.empty())
        {
            throw runtime_error("The video codec wasn't specified.");
        }

        // find the video encoder
        AVCodec *videoCodec = NULL;
        if (!this->videoParam.videoCodecName.empty())
        {
            // use the codec name preferentially if it is specified in the input param
            videoCodec = avcodec_find_encoder_by_name(this->videoParam.videoCodecName.c_str());
        }
        else
        {
            // otherwise, use the codec guessed from the filename
            videoCodec = avcodec_find_encoder(outputFormat->video_codec);
        }
        if (!videoCodec)
        {
            throw runtime_error("Video codec not found.");
        }

        // add the video stream with stream id 0
        this->videoStream = av_new_stream(this->outputContext, 0);
        if (!this->videoStream)
        {
            throw bad_alloc("Couldn't allocate video stream.");
        }

        // set the parameters for video codec context
        AVCodecContext *videoCodecContext = this->videoStream->codec;
        videoCodecContext->codec_id       = videoCodec->id;
        videoCodecContext->codec_type     = CODEC_TYPE_VIDEO;
        videoCodecContext->bit_rate       = this->videoParam.bitRate;
        videoCodecContext->width          = this->videoParam.width;
        videoCodecContext->height         = this->videoParam.height;
        videoCodecContext->time_base.den  = this->videoParam.frameRate;
        videoCodecContext->time_base.num  = 1;

        // set the PixelFormat of the target encoded video
        if (videoCodec->pix_fmts)
        {
            // try to find the PixelFormat required by the input param,
            // use the default PixelFormat directly if required format not found
            const enum PixelFormat *p= videoCodec->pix_fmts;
            for ( ; *p != PIX_FMT_NONE; p ++)
            {
                if (*p == this->videoParam.pixelFormat)
                    break;
            }
            if (*p == PIX_FMT_NONE)
                videoCodecContext->pix_fmt = videoCodec->pix_fmts[0];
            else
                videoCodecContext->pix_fmt = *p;
        }

        // some formats want stream headers to be separate
        if (outputFormat && outputFormat->video_codec != CODEC_ID_NONE)
        {
            if (   !strcmp(this->outputContext->oformat->name, "mp4")
                || !strcmp(this->outputContext->oformat->name, "mov") 
                || !strcmp(this->outputContext->oformat->name, "3gp") )
            {
                videoCodecContext->flags |= CODEC_FLAG_GLOBAL_HEADER;
            }
        }

        // open the video codec
        if (avcodec_open(videoCodecContext, videoCodec) < 0)
        {
            throw runtime_error("Could not open the video codec.");
        }

        // allocate the output buffer
        // the maximum possible buffer size could be the raw bmp format with R/G/B/A
        this->videoBufferSize = 4 * this->videoParam.width * this->videoParam.height;
        this->videoBuffer     = (uint8_t*)(av_malloc(this->videoBufferSize));

        // allocate the temporal video frame buffer for pixel format conversion if needed
        if (this->videoParam.pixelFormat != videoCodecContext->pix_fmt)
        {
            this->videoFrame = (AVPicture *)av_malloc(sizeof(AVPicture));
            if (   this->videoFrame == NULL
                || avpicture_alloc(this->videoFrame, videoCodecContext->pix_fmt, videoCodecContext->width, videoCodecContext->height) < 0 )
            {
                throw bad_alloc("Couldn't allocate the video frame for pixel format conversion.");
            }
        }
    }

    // audio related initialization if necessary
    if (this->encodeAudio)
    {
        // validate the audio codec
        if ((!outputFormat || outputFormat->audio_codec == CODEC_ID_NONE) && this->audioParam.audioCodecName.empty())
        {
            throw runtime_error("The audio codec wasn't specified.");
        }

        // find the audio encoder
        AVCodec *audioCodec = NULL;
        if (!this->audioParam.audioCodecName.empty())
        {
            // use the codec name preferentially if it is specified in the input param
            audioCodec = avcodec_find_encoder_by_name(this->audioParam.audioCodecName.c_str());
        }
        else
        {
            // otherwise, use the codec guessed from the filename
            audioCodec = avcodec_find_encoder(outputFormat->audio_codec);
        }
        if (!audioCodec)
        {
            throw runtime_error("Audio codec not found.");
        }

        // add the audio stream with stream id 1
        this->audioStream = av_new_stream(this->outputContext, 1);
        if (!this->audioStream)
        {
            throw bad_alloc("Couldn't allocate audio stream.");
        }

        // set the parameters for audio codec context
        AVCodecContext *audioCodecContext = this->audioStream->codec;
        audioCodecContext->codec_id       = audioCodec->id;
        audioCodecContext->codec_type     = CODEC_TYPE_AUDIO;
        audioCodecContext->bit_rate       = this->audioParam.bitRate;
        audioCodecContext->sample_rate    = this->audioParam.sampleRate;
        audioCodecContext->channels       = this->audioParam.channels;

        // open the audio codec
        if (avcodec_open(audioCodecContext, audioCodec) < 0)
        {
            throw runtime_error("Could not open the audio codec.");
        }

        // TODO: how to determine the buffer size?
        // allocate the output buffer
        this->audioBufferSize = 4 * MAX_AUDIO_PACKET_SIZE;
        this->audioBuffer     = (uint8_t*)(av_malloc(this->audioBufferSize));
    }

    // set the output parameters (must be done even if no parameters).
    if (this->hasOutput)
    {
        if (av_set_parameters(this->outputContext, NULL) < 0)
        {
            throw runtime_error("Invalid output format parameters.");
        }
    }

    //dump_format(this->outputContext, 0, this->fileName.c_str(), 1);

    // open the output file, if needed
    if (this->hasOutput)
    {
        if (url_fopen(&this->outputContext->pb, fileName, URL_WRONLY) < 0)
        {
            throw runtime_error(string("Could not open the file: ") + fileName);
        }

        // write the stream header, if any
        if (av_write_header(this->outputContext))
        {
            throw runtime_error("Could not write the video header.");
        }
    }

    this->opened = true;
}

void FFmpegEncoder::close()
{
    if (!this->opened)
    {
        return;
    }

    if (this->hasOutput)
    {
        // write the trailer, and close the output file
        av_write_trailer(this->outputContext);
        url_fclose(&this->outputContext->pb);
    }

    if (this->encodeVideo)
    {
        // close the video stream and codec
        avcodec_close(this->videoStream->codec);
        av_freep(&this->videoStream->codec);
        av_freep(&this->videoStream);
        av_freep(&this->videoBuffer);
        this->videoBufferSize = 0;
        if (this->videoFrame != NULL)
        {
            avpicture_free(this->videoFrame);
            av_freep(&this->videoFrame);
        }
    }

    if (this->encodeAudio)
    {
        // close the audio stream and codec
        avcodec_close(this->audioStream->codec);
        av_freep(&this->audioStream->codec);
        av_freep(&this->audioStream);
        av_freep(&this->audioBuffer);
        this->audioBufferSize = 0;
    }

    av_freep(&this->outputContext);

    this->opened = false;
    this->hasOutput = false;
}


//////////////////////////////////////////////////////////////////////////
//
//  Private Methods
//
//////////////////////////////////////////////////////////////////////////

int FFmpegEncoder::encodeVideoData(AVPicture *picture)
{
    AVCodecContext *videoCodecContext = this->videoStream->codec;

    AVFrame *frame = avcodec_alloc_frame();
    if (!frame)
    {
        throw runtime_error("Could not allocate frame.");
    };

    // convert the pixel format if needed
    if (this->videoParam.pixelFormat != videoCodecContext->pix_fmt)
    {
        if (this->convertPixFmt(picture, this->videoFrame, &this->videoParam, videoCodecContext) != 0)
        {
            throw runtime_error("Fail to convert the image pixel format.");
        }
        // fill the frame
        *(AVPicture *)frame = *this->videoFrame;
    }
    else
    {
        // fill the frame
        *(AVPicture *)frame = *picture;
    }

    frame->pts = AV_NOPTS_VALUE;

    // encode the frame
    int encodedSize = avcodec_encode_video(videoCodecContext, this->videoBuffer, this->videoBufferSize, frame);

    av_free(frame);

    if (encodedSize < 0)
    {
        throw runtime_error("Error while encoding the video frame.");
    }
    else
    {
        return encodedSize;
    }
}

void FFmpegEncoder::writeVideoData(uint8_t *packetData, int packetSize)
{
    AVPacket packet;
    av_init_packet(&packet);

    packet.pts= av_rescale_q(this->videoStream->codec->coded_frame->pts, this->videoStream->codec->time_base, this->videoStream->time_base);
    if (this->videoStream->codec->coded_frame->key_frame)
    {
        packet.flags |= PKT_FLAG_KEY;
    }
    packet.stream_index = this->videoStream->index;
    packet.data = packetData;
    packet.size = packetSize;

    // write the compressed frame in the media file
    int success = av_write_frame(this->outputContext, &packet);

    av_free_packet(&packet);

    if (success < 0)
    {
        throw runtime_error("Error while writing video frame.");
    }
}

int FFmpegEncoder::convertPixFmt(AVPicture *srcPic, AVPicture *dstPic, const FFmpegVideoParam *srcParam, const AVCodecContext *dstContext)
{
    static SwsContext *img_convert_ctx = NULL;

    if (img_convert_ctx == NULL)
    {
        img_convert_ctx = sws_getContext(
            srcParam->width, srcParam->height, srcParam->pixelFormat,
            dstContext->width, dstContext->height, dstContext->pix_fmt,
            SWS_BICUBIC, NULL, NULL, NULL);
    }

    if (img_convert_ctx == NULL)
    {
        throw runtime_error("Cannot initialize the image conversion context.");
    }

    return sws_scale(img_convert_ctx, srcPic->data, srcPic->linesize, 0, srcParam->height, dstPic->data, dstPic->linesize);
}

int FFmpegEncoder::encodeAudioData(short *frameData, int dataSize)
{
    // the output size of the buffer which stores the encoded data
    int audioSize = this->audioBufferSize;

    if (this->audioStream->codec->frame_size <=1 && dataSize > 0)
    {
        // For PCM related codecs, the output size of the encoded data is
        // calculated from the size of the input audio frame.
        audioSize = dataSize;

        // The following codes are used for calculating "short" size from original "sample" size.
        // The codes are not needed any more because now the input size is already in "short" unit.

        // calculated the PCM size from input data size
        //switch(this->audioStream->codec->codec_id)
        //{
        //    case CODEC_ID_PCM_S32LE:
        //    case CODEC_ID_PCM_S32BE:
        //    case CODEC_ID_PCM_U32LE:
        //    case CODEC_ID_PCM_U32BE:
        //        audioSize <<= 1;
        //        break;
        //    case CODEC_ID_PCM_S24LE:
        //    case CODEC_ID_PCM_S24BE:
        //    case CODEC_ID_PCM_U24LE:
        //    case CODEC_ID_PCM_U24BE:
        //    case CODEC_ID_PCM_S24DAUD:
        //        audioSize = audioSize / 2 * 3;
        //        break;
        //    case CODEC_ID_PCM_S16LE:
        //    case CODEC_ID_PCM_S16BE:
        //    case CODEC_ID_PCM_U16LE:
        //    case CODEC_ID_PCM_U16BE:
        //        break;
        //    default:
        //        audioSize >>= 1;
        //        break;
        //}
    }

    // encode the frame
    int encodedSize = avcodec_encode_audio(this->audioStream->codec, this->audioBuffer, audioSize, frameData);

    if (encodedSize < 0)
    {
        throw runtime_error("Error while encoding the audio frame.");
    }
    else
    {
        return encodedSize;
    }
}

void FFmpegEncoder::writeAudioData(uint8_t *packetData, int packetSize)
{
    AVPacket packet;
    av_init_packet(&packet);

    if (this->audioStream->codec && this->audioStream->codec->coded_frame->pts != AV_NOPTS_VALUE)
    {
        packet.pts= av_rescale_q(this->audioStream->codec->coded_frame->pts, this->audioStream->codec->time_base, this->audioStream->time_base);
    }
    packet.flags |= PKT_FLAG_KEY;
    packet.stream_index = this->audioStream->index;
    packet.data = packetData;
    packet.size = packetSize;

    // write the compressed frame in the media file
    int success = av_write_frame(this->outputContext, &packet);

    av_free_packet(&packet);

    if (success < 0)
    {
        throw runtime_error("Error while writing audio frame.");
    }
}