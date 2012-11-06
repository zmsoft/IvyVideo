ROOT_PATH := $(call my-dir)/../../..
LOCAL_PATH := $(ROOT_PATH)/libavcodec
EXTERNAL := $(ROOT_PATH)/..
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

TARGET_ARCH := arm
include $(LOCAL_PATH)/../av.mk

LOCAL_SRC_FILES := $(FFFILES)

LOCAL_C_INCLUDES :=		\
	$(LOCAL_PATH)		\
	$(LOCAL_PATH)/..	\
	$(LOCAL_PATH)/../libx264

LOCAL_CFLAGS += $(FFCFLAGS)

LOCAL_SHARED_LIBRARIES := 
LOCAL_STATIC_LIBRARIES := $(FFLIBS)

LOCAL_LDLIBS := -llog -lm -L../obj/local/armeabi-v7a -lavutil -lx264

LOCAL_MODULE := $(FFNAME)

include $(BUILD_SHARED_LIBRARY)
