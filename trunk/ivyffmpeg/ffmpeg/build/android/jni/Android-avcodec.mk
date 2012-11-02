ROOT_PATH := $(call my-dir)/../../..
LOCAL_PATH := $(ROOT_PATH)/libavcodec
EXTERNAL := $(ROOT_PATH)/..
include $(CLEAR_VARS)

TARGET_ARCH := arm
include $(LOCAL_PATH)/../av.mk

LOCAL_SRC_FILES := $(FFFILES)

LOCAL_C_INCLUDES :=		\
	$(LOCAL_PATH)		\
	$(LOCAL_PATH)/..

LOCAL_CFLAGS += $(FFCFLAGS)

LOCAL_SHARED_LIBRARIES :=
LOCAL_STATIC_LIBRARIES := $(FFLIBS)

LOCAL_LDLIBS := -lm -L../obj/local/armeabi-v7a -lavutil

LOCAL_MODULE := $(FFNAME)

include $(BUILD_SHARED_LIBRARY)
