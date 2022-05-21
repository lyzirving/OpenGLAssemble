//
// Created by lyzirving on 2022/5/21.
//
#ifndef OPENGLASSEMBLE_WINDOWSURFACE_H
#define OPENGLASSEMBLE_WINDOWSURFACE_H

#include <android/native_window_jni.h>
#include <string>
#include <memory>

#include <EglCore.h>

class WindowSurface {
public:
    WindowSurface(const std::shared_ptr<EglCore> &eglCore, ANativeWindow *window, const char* name);
    ~WindowSurface();

    EGLSurface getSurface();
    int32_t getWidth();
    int32_t getHeight();
    const std::string &getName();
    bool isPrepared();
    bool makeCurrent();
    bool prepare();
    void release();
    bool swapBuffer();

private:
    std::shared_ptr<EglCore> mEglCore;
    ANativeWindow *mWindow;
    EGLSurface mSurface;
    std::string mName;
    int32_t mWidth;
    int32_t mHeight;
};

#endif //OPENGLASSEMBLE_WINDOWSURFACE_H
