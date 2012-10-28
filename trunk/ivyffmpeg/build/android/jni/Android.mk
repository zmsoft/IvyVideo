ROOT_PATH := $(call my-dir)/../../..
EXT_PATH := $(ROOT_PATH)/..
LOCAL_PATH := $(ROOT_PATH)/src
TARGET_ARCH := arm

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
        FFmpegDecoder.cpp  \
        FFmpegEncoder.cpp  \
	FFmpegVideoParam.cpp \
	FFmpegAudioParam.cpp 


LOCAL_SHARED_LIBRARIES := libz
LOCAL_STATIC_LIBRARIES := libavcodec libavformat libavutil

LOCAL_MODULE:= libivyffmpeg

EXT_LIB := $(ROOT_PATH)/ffmpeg
LOCAL_C_INCLUDES := 			\
	$(EXT_LIB) 		\
	$(LOCAL_PATH)

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DTARGET_CONFIG=\"config-$(TARGET_ARCH).h\"

#include $(BUILD_PLUGIN_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
