#include <jni.h>
#include <string>

#include "LogUtil.h"

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LogE("failed to load jni env");
        return JNI_ERR;
    }
    LogI("succeed to load jni env");
    return JNI_VERSION_1_6;
}
