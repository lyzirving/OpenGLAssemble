//
// Created by lyzirving on 2022/5/19.
//
#include <jni.h>

#include "RendererContext.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "native_RendererContext"

#define JAVA_CLASS "com/lyzirving/opengl/assemble/renderer/RendererContext"

static jlong nativeCreateContext(JNIEnv *env, jclass clazz, jstring jName) {
    const char* name = env->GetStringUTFChars(jName, nullptr);
    auto *context = new RendererContext(name);
    env->ReleaseStringUTFChars(jName, name);
    return reinterpret_cast<jlong>(context);
}

static JNINativeMethod methods[] = {
        {
                "nCreateContext",
                "(Ljava/lang/String;)J",
                (void *) nativeCreateContext
        },
};

bool register_native_RendererContext(JNIEnv *env) {
    int count = sizeof(methods) / sizeof(methods[0]);
    jclass javaClass = env->FindClass(JAVA_CLASS);
    if(!javaClass) {
        LogE("fail to find java class %s", JAVA_CLASS);
        goto error;
    }
    if (env->RegisterNatives(javaClass, methods, count) < 0) {
        LogE("fail to register jni methods for class %s", JAVA_CLASS);
        goto error;
    }
    LogI("succeed to load class %s", JAVA_CLASS);
    return true;

    error:
    return false;
}

