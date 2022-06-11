//
// Created by lyzirving on 2022/5/19.
//
#include <android/native_window_jni.h>

#include "RendererContext.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "native_RendererContext"

#define JAVA_CLASS "com/lyzirving/opengl/assemble/renderer/RendererContext"

enum RendererType : uint8_t {
    BASE = 0x01,
};

static struct {
    jclass mClazz;
    jmethodID mMethodOnThreadQuit;
} gRendererContextClassInfo;

static jlong nativeCreateContext(JNIEnv *env, jclass clazz, jstring jName,
                                 jobject assetsManager, jint type) {
    const char *name = env->GetStringUTFChars(jName, nullptr);
    RendererContext *ptr{nullptr};
    switch (type) {
        case RendererType::BASE: {
            ptr = new RendererContext(name);
            break;
        }
        default: {
            break;
        }
    }
    env->ReleaseStringUTFChars(jName, name);
    GlHelper::setAssetsManager(env, assetsManager);
    return reinterpret_cast<jlong>(ptr);
}

static void nativeSendMessage(JNIEnv *env, jclass clazz, jlong address, jint what, jint arg0, jint arg1) {
    auto *context = reinterpret_cast<RendererContext *>(address);
    if(context) {
        context->sendMessage(what);
    }
}

static void nativeRelease(JNIEnv *env, jclass clazz, jlong address) {
    auto *context = reinterpret_cast<RendererContext *>(address);
    GlHelper::release();
    if(context) {
        context->requestQuit();
    }
}

static void nativeRegisterWindow(JNIEnv *env, jclass clazz, jlong address,
                                 jstring jName, jobject jWindow) {
    auto *context = reinterpret_cast<RendererContext *>(address);
    if (context) {
        const char* name = env->GetStringUTFChars(jName, nullptr);
        ANativeWindow *window = ANativeWindow_fromSurface(env, jWindow);
        if (!context->registerWindow(name, window)) {
            ANativeWindow_release(window);
        }
        env->ReleaseStringUTFChars(jName, name);
    }
}

static void nativeRemoveWindow(JNIEnv *env, jclass clazz, jlong address, jstring jName) {
    auto *context = reinterpret_cast<RendererContext *>(address);
    if (context) {
        const char* name = env->GetStringUTFChars(jName, nullptr);
        context->sendMessage(MessageId::MESSAGE_REMOVE_WINDOW, 0, 0, name);
        env->ReleaseStringUTFChars(jName, name);
    }
}

static JNINativeMethod methods[] = {
        {
                "nCreateContext",
                "(Ljava/lang/String;Landroid/content/res/AssetManager;I)J",
                (void *) nativeCreateContext
        },
        {
                "nSendMessage",
                "(JIII)V",
                (void *) nativeSendMessage
        },
        {
                "nRelease"  ,
                "(J)V",
                (void *) nativeRelease
        },
        {
                "nRegisterWindow"  ,
                "(JLjava/lang/String;Landroid/view/Surface;)V",
                (void *) nativeRegisterWindow
        },
        {
                "nRemoveWindow"  ,
                "(JLjava/lang/String;)V",
                (void *) nativeRemoveWindow
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
    gRendererContextClassInfo.mClazz = static_cast<jclass>(env->NewGlobalRef(javaClass));
    LogI("succeed to load class %s", JAVA_CLASS);
    return true;

    error:
    return false;
}

