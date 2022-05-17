//
// Created by lyzirving on 2022/5/17.
//
#ifndef OPENGLASSEMBLE_LOGUTIL_H
#define OPENGLASSEMBLE_LOGUTIL_H

#include <android/log.h>
#include <string.h>

#define TAG "Assembler"
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)

#define LogD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG,\
                          "[%s][%s][%d]: " format, __FILENAME__,\
                          __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LogI(format, ...) __android_log_print(ANDROID_LOG_INFO, TAG,\
                          "[%s][%s][%d]: " format, __FILENAME__,\
                          __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LogW(format, ...) __android_log_print(ANDROID_LOG_WARN, TAG,\
                          "[%s][%s][%d]: " format, __FILENAME__,\
                          __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LogE(format, ...) __android_log_print(ANDROID_LOG_ERROR, TAG,\
                          "[%s][%s][%d]: " format, __FILENAME__,\
                          __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif //OPENGLASSEMBLE_LOGUTIL_H
