ROOT_PATH := $(call my-dir)/../../..
LOCAL_PATH := $(ROOT_PATH)/libavformat
EXTERNAL := $(ROOT_PATH)/..
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../av.mk

LOCAL_SRC_FILES := $(FFFILES)

LOCAL_C_INCLUDES :=		\
	$(LOCAL_PATH)		\
	$(LOCAL_PATH)/..

LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_CFLAGS += -include "string.h" -Dipv6mr_interface=ipv6mr_ifindex

LOCAL_SHARED_LIBRARIES := 
LOCAL_STATIC_LIBRARIES := $(FFLIBS)

LOCAL_LDLIBS := -L../obj/local/armeabi-v7a -lavutil -lavcodec

LOCAL_MODULE := $(FFNAME)

include $(BUILD_SHARED_LIBRARY)
