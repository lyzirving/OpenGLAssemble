//
// Created by lyzirving on 2022/5/21.
//
#include "WindowSurface.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WindowSurface"

WindowSurface::WindowSurface(EglCore *eglCore, ANativeWindow *window)
        : mEglCore(eglCore), mWindow(window), mSurface(EGL_NO_SURFACE),
          mWidth(0), mHeight(0) {
}

WindowSurface::~WindowSurface() {
    release();
}

EGLSurface WindowSurface::getSurface() {
    return mSurface;
}

int32_t WindowSurface::getWidth() {
    return mWidth;
}

int32_t WindowSurface::getHeight() {
    return mHeight;
}

bool WindowSurface::isPrepared() {
    return mSurface != EGL_NO_SURFACE;
}

bool WindowSurface::makeCurrent() {
    bool success{false};
    if(mEglCore == nullptr) {
        LogE("EglCore is null");
        goto done;
    }
    if (mSurface == EGL_NO_SURFACE) {
        LogE("surface is null");
        goto done;
    }
    if (mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogE("display is null");
        goto done;
    }
    if (mEglCore->getEglContext() == EGL_NO_CONTEXT) {
        LogE("context is null");
        goto done;
    }
    if (!eglMakeCurrent(mEglCore->getDisplay(), mSurface, mSurface, mEglCore->getEglContext())) {
        LogE("fail to make current, err code = 0x%x", eglGetError());
        goto done;
    }
    success = true;
    done:
    return success;
}

bool WindowSurface::prepare() {
    if(isPrepared()) {
        LogI("already prepared");
        return true;
    }
    if(mEglCore == nullptr) {
        LogI("EglCore is null");
        return false;
    }
    if(mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogI("display is null");
        return false;
    }
    if(mWindow == nullptr) {
        LogI("window is null");
        return false;
    }

    const EGLint attribute[] = {EGL_NONE};
    mSurface = eglCreateWindowSurface(mEglCore->getDisplay(), mEglCore->getEglConfig(),
                                      mWindow, attribute);
    EGLint ret = eglGetError();
    if (ret != EGL_SUCCESS) {
        LogE("fail to prepare, err code = 0x%x", ret);
        mSurface = EGL_NO_SURFACE;
    } else {
        mWidth = ANativeWindow_getWidth(mWindow);
        mHeight = ANativeWindow_getHeight(mWindow);
        LogI("success");
    }
    return ret == EGL_SUCCESS;
}

void WindowSurface::release() {
    if (mSurface != EGL_NO_SURFACE) {
        eglMakeCurrent(mEglCore->getDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if(eglDestroySurface(mEglCore->getDisplay(), mSurface)) {
            LogI("destroy surface");
        } else {
            LogE("fail to destroy surface, err code = 0x%x", eglGetError());
        }
    }
    if (mWindow != nullptr) {
        LogI("release native window");
        ANativeWindow_release(mWindow);
    }
    // EglCore should be released outside
    //todo use weak ptr?
    mEglCore = nullptr;
    mWindow = nullptr;
    mSurface = EGL_NO_SURFACE;
}

bool WindowSurface::swapBuffer() {
    bool success{false};
    if(mEglCore == nullptr) {
        LogE("EglCore is null");
        goto done;
    }
    if (mSurface == EGL_NO_SURFACE) {
        LogE("surface is null");
        goto done;
    }
    if (mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogE("display is null");
        goto done;
    }
    if (!eglSwapBuffers(mEglCore->getDisplay(), mSurface)) {
        LogE("fail to swap buffer, err code = 0x%x", eglGetError());
        goto done;
    }
    success = true;
    done:
    return success;
}

