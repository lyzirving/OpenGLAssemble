#include <jni.h>
#include <string>

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "JniLoader"

extern bool register_native_RendererContext(JNIEnv* env);

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        LogE("failed to load jni env");
        goto error;
    }
    if(!register_native_RendererContext(env))
        goto error;

    LogI("succeed to load jni env");
    return JNI_VERSION_1_6;

    error:
    return JNI_ERR;
}
