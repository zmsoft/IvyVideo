ROOT_PATH := $(call my-dir)/../../..
EXT_PATH := $(ROOT_PATH)/ffmpeg
LOCAL_PATH := $(ROOT_PATH)/src
TARGET_ARCH := arm

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
        FFmpegDecoder.cpp  \
        FFmpegEncoder.cpp  \
	FFmpegVideoParam.cpp \
	FFmpegAudioParam.cpp 

LOCAL_SHARED_LIBRARIES := 
LOCAL_STATIC_LIBRARIES := libavcodec libavformat libavutil

LOCAL_MODULE := libivyffmpeg

LOCAL_C_INCLUDES := 		\
	$(LOCAL_PATH)		\
	$(EXT_PATH) 

LOCAL_CFLAGS := 

include $(BUILD_STATIC_LIBRARY)
