//
// Created by lyzirving on 2022/5/21.
//
#ifndef OPENGLASSEMBLE_WINDOWSURFACE_H
#define OPENGLASSEMBLE_WINDOWSURFACE_H

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <EglCore.h>

class WindowSurface {
public:
    WindowSurface(EglCore *eglCore, ANativeWindow *window);
    ~WindowSurface();

    EGLSurface getSurface();
    int32_t getWidth();
    int32_t getHeight();
    bool isPrepared();
    bool makeCurrent();
    bool prepare();
    void release();
    bool swapBuffer();

private:
    EglCore *mEglCore;
    ANativeWindow *mWindow;
    EGLSurface mSurface;
    int32_t mWidth;
    int32_t mHeight;
};

#endif //OPENGLASSEMBLE_WINDOWSURFACE_H
