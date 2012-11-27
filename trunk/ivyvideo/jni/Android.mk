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

LOCAL_ARM_MODE := arm

LOCAL_MODULE    := ivyvideo
LOCAL_SRC_FILES := ../src/jni_ivymirage.cpp \
	../src/Runnable.cpp  \
	../src/SampleAllocator.cpp  \
	../src/Timer.cpp  \
	../src/LogTrace.cpp  \
	../src/IvyClient.cpp \
	../src/IvyUtil.cpp \
	../src/RtpPacket.cpp \
	../src/MediaSession.cpp \
	../src/VideoDecode.cpp  \
	../src/VideoEncode.cpp

LOCAL_STATIC_LIBRARIES := 
LOCAL_SHARED_LIBRARIES := 

LOCAL_LDLIBS := -llog
LOCAL_LDLIBS += -L$(EXT_PATH)/ivyffmpeg/build/libs/armeabi-v7a -livyffmpeg
LOCAL_LDLIBS += -L$(EXT_PATH)/sgsclient -lsgsclient

LOCAL_C_INCLUDES := 		\
	$(LOCAL_PATH)		\
	$(EXT_PATH)/ivyffmpeg/ffmpeg 		\
	$(EXT_PATH)/ivyffmpeg/src 		\
	$(EXT_PATH)/sgsclient

LOCAL_CFLAGS := -DANDROID

include $(BUILD_SHARED_LIBRARY)
