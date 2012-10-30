# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
EXT_PATH := ../..


include $(CLEAR_VARS)
LOCAL_MODULE := libivyffmpeg
LOCAL_SRC_FILES := $(EXT_PATH)/ivyffmpeg/build/android/obj/local/armeabi-v7a/libivyffmpeg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := $(EXT_PATH)/ivyffmpeg/ffmpeg/build/android/obj/local/armeabi-v7a/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := $(EXT_PATH)/ivyffmpeg/ffmpeg/build/android/obj/local/armeabi-v7a/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := $(EXT_PATH)/ivyffmpeg/ffmpeg/build/android/obj/local/armeabi-v7a/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := $(EXT_PATH)/ivyffmpeg/ffmpeg/build/android/obj/local/armeabi-v7a/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := ivyvideo
LOCAL_SRC_FILES := ../src/ivyvideo-jni.c \
	../src/Runnable.cpp  \
	../src/SampleAllocator.cpp  \
	../src/Timer.cpp  \
	../src/VideoDecode.cpp  \
	../src/VideoEncode.cpp

LOCAL_STATIC_LIBRARIES := libivyffmpeg libavcodec libavformat libavutil libswscale

LOCAL_SHARED_LIBRARIES := 

LOCAL_LDLIBS := -lz

LOCAL_C_INCLUDES := 		\
	$(LOCAL_PATH)		\
	$(EXT_PATH)/ivyffmpeg/ffmpeg 		\
	$(EXT_PATH)/ivyffmpeg/src 		

LOCAL_CFLAGS := 

include $(BUILD_SHARED_LIBRARY)
