//
// Created by lyzirving on 2022/5/21.
//
#ifndef OPENGLASSEMBLE_EGLCORE_H
#define OPENGLASSEMBLE_EGLCORE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

class EglCore {
public:
    EglCore();
    ~EglCore();

    EGLDisplay getDisplay();
    EGLConfig getEglConfig();
    EGLContext getEglContext();

    bool makeCurrent(EGLSurface draw = EGL_NO_SURFACE, EGLSurface read = EGL_NO_SURFACE);
    void release();
    bool prepare(EGLContext shareContext = EGL_NO_CONTEXT);
    void swapBuffer(bool block = false);

private:
    EGLDisplay mEglDisplay;
    EGLContext mEglCtx;
    EGLConfig mEglConfig;
};

#endif //OPENGLASSEMBLE_EGLCORE_H
