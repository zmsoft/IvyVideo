#ifndef _LOGTRACE_H_
#define _LOGTRACE_H_

#ifdef ANDROID
#include <android/log.h>
#define IVYTAG "IvyMirageNative"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, IVYTAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, IVYTAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, IVYTAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, IVYTAG, __VA_ARGS__))

#else

#include <stdio.h>
#define IVYTAG "IvyMirageNative - "

#define LOGD(...) fprintf(stderr, "[D] - " IVYTAG __VA_ARGS__)
#define LOGI(...) fprintf(stderr, "[I] - " IVYTAG __VA_ARGS__)
#define LOGW(...) fprintf(stderr, "[W] - " IVYTAG __VA_ARGS__)
#define LOGE(...) fprintf(stderr, "[E] - " IVYTAG __VA_ARGS__)
#endif

#endif
