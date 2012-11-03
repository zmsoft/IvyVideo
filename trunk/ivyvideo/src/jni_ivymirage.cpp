#include <stdlib.h>
#include <jni.h>

#include "IvyCommon.h"
#include "IvyVideo.h"
#include "LogTrace.h"

#define JNI_VERSION 	JNI_VERSION_1_2
#define JNIREG_CLASS 	"com/ivysee/mirage/Engine"

JNIEXPORT jstring JNICALL native_hello(JNIEnv *env, jclass clazz)
{
    LOGI("hello in c native code.");
    return env->NewStringUTF("hello world returned.");
}

JNIEXPORT void JNICALL native_init(JNIEnv *env, jclass clazz)
{
    LOGI("native_init called, begin");
}

JNIEXPORT void JNICALL native_uninit(JNIEnv *env, jclass clazz)
{
    LOGI("native_uninit called, begin");
}

JNIEXPORT void JNICALL native_rawvideo(JNIEnv *env, jclass clazz, 
	jint handle, jbyteArray array, jint len, jint fmt, jint width, jint height, jint orientation)
{
    LOGI("native_rawvideo called, begin");
    IvyVideoEncode *pEnc = (IvyVideoEncode *)handle;
    return_if_fail(pEnc != NULL && len > 0);

    RawFrameFormat frameFormat;
    frameFormat.fmt = fmt;
    frameFormat.width = width;
    frameFormat.height = height;
    frameFormat.orientation = orientation;

    jbyte *data = env->GetByteArrayElements(array, NULL);
    return_if_fail(data != NULL);
    pEnc->onRawFrame((char *)data, len, frameFormat);
    env->ReleaseByteArrayElements(array, data, JNI_ABORT);
    LOGI("native_rawvideo called, end");
}

/**
 * Table of methods associated with a single class.
 */
static JNINativeMethod gMethods[] = {
    { "native_hello", "()Ljava/lang/String;", (void*)native_hello },
    { "native_rawvideo", "(I[BIIIII)V", (void*)native_rawvideo },
};

/*
 * Register several native methods for one class.
 */
static int register_native_methods(JNIEnv* env, const char* className,
	JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
	LOGE("cannot find class: %s", className);
	return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
	LOGE("env->RegisterNativescannot faileds");
	return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 */
static int register_natives(JNIEnv* env)
{
    if (!register_native_methods(env, JNIREG_CLASS, gMethods, 
		sizeof(gMethods) / sizeof(gMethods[0]))) {
	return JNI_FALSE;
    }

    return JNI_TRUE;
}

/**
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint ret = -1;
    JNIEnv* env = NULL;

    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION) != JNI_OK) {
	LOGE("JNI_OnLoad called, GetEnv failed");
	return ret;
    }

    if (env == NULL || !register_natives(env)) {
	LOGE("JNI_OnLoad called, no env or register natives failed");
	return ret;
    }

    ret = JNI_VERSION;
    return ret;
}

