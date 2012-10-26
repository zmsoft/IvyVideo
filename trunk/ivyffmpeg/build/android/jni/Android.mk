ROOT_PATH := $(call my-dir)/../../..
EXT_PATH := $(ROOT_PATH)/..
LOCAL_PATH := $(ROOT_PATH)/src

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	EncodeProfile.cpp  \
        FFMpegCanvas.cpp   \
        FFMpegDecoder.cpp  \
        FFMpegFifo.cpp     \
        FFMpegVideoDecoder.cpp  \
	FFMpegAudioConverter.cpp  \
        FFMpegCodecDecoder.cpp  \
        FFMpegDemuxer.cpp  \
        FFMpegMuxer.cpp    \
        myFFMpeg.cpp       \
	FFMpegBitmapConverter.cpp \
        FFMpegCodecEncoder.cpp  \
        FFMpegEncoder.cpp  \
        FFMpegReSampler.cpp


LOCAL_SHARED_LIBRARIES := libz
LOCAL_STATIC_LIBRARIES := libavcodec libavformat libavutil

LOCAL_MODULE:= libivyffmpeg

EXT_LIB := $(ROOT_PATH)/ffmpeg
LOCAL_C_INCLUDES := 			\
	$(EXT_LIB) 		\
	$(EXT_LIB)/libavcodec 	\
	$(EXT_LIB)/libavformat  \
	$(EXT_LIB)/libavutil	\
	$(LOCAL_PATH)/../include

LOCAL_CFLAGS := -DHAVE_CONFIG_H			

#include $(BUILD_PLUGIN_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
