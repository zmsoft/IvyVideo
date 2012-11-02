#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>

#include "VideoEncoder.h"

#define JNI_VERSION 	JNI_VERSION_1_2
#define JNIREG_CLASS 	"com/ivysee/mirage"

JNIEXPORT jstring JNICALL native_hello(JNIEnv *env, jclass clazz)
{
	printf("hello in c native code.\n");
	return (*env)->NewStringUTF(env, "hello world returned.");
}

JNIEXPORT jvoid JNICALL native_init(JNIEnv *env, jclass clazz)
{
}

JNIEXPORT jvoid JNICALL native_uninit(JNIEnv *env, jclass clazz)
{
}

JNIEXPORT jvoid JNICALL native_rawdata(JNIEnv *env, jclass clazz, jobject handle, 
	jarray data, jint size, jint fmt, jint width, jint height, jint orientation, jint facing)
{
	CVideoEncode *pEnc = (CVideoEncode *)handle;
	if (!pEnc || size <= 0) {
		return;
	}
}

/**
* Table of methods associated with a single class.
*/
static JNINativeMethod gMethods[] = {
	{ "hello", "()Ljava/lang/String;", (void*)native_hello },
};

/*
* Register several native methods for one class.
*/
static int register_native_methods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}

	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
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

	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION) != JNI_OK) {
		return ret;
	}

	if (env == NULL || !register_natives(env)) {
		return ret;
	}

	ret = JNI_VERSION;
	return ret;
}

