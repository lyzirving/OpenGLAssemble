//
// Created by lyzirving on 2022/5/21.
//
#include "WindowSurface.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "WindowSurface"

WindowSurface::WindowSurface(const std::shared_ptr<EglCore> &eglCore, ANativeWindow *window,
                             const char *name) : mEglCore(eglCore), mWindow(window),
                                                 mSurface(EGL_NO_SURFACE), mName(name),
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

const std::string &WindowSurface::getName() {
    return mName;
}

bool WindowSurface::isPrepared() {
    return mSurface != EGL_NO_SURFACE;
}

bool WindowSurface::makeCurrent() {
    bool success{false};
    if (mEglCore == nullptr) {
        LogE("WindowSurface(%s) EglCore is null", mName.c_str());
        goto done;
    }
    if (mSurface == EGL_NO_SURFACE) {
        LogE("WindowSurface(%s), surface is null", mName.c_str());
        goto done;
    }
    if (mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogE("WindowSurface(%s) display is null", mName.c_str());
        goto done;
    }
    if (mEglCore->getEglContext() == EGL_NO_CONTEXT) {
        LogE("WindowSurface(%s), context is null", mName.c_str());
        goto done;
    }
    if (!eglMakeCurrent(mEglCore->getDisplay(), mSurface, mSurface, mEglCore->getEglContext())) {
        LogE("WindowSurface(%s) fail to make current, err code = 0x%x",
             mName.c_str(), eglGetError());
        goto done;
    }
    success = true;
    done:
    return success;
}

bool WindowSurface::prepare() {
    if (isPrepared()) {
        LogI("WindowSurface(%s), already prepared", mName.c_str());
        return true;
    }
    if (mEglCore == nullptr) {
        LogI("WindowSurface(%s), EglCore is null", mName.c_str());
        return false;
    }
    if (mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogI("WindowSurface(%s), display is null", mName.c_str());
        return false;
    }
    if (mWindow == nullptr) {
        LogI("WindowSurface(%s), window is null", mName.c_str());
        return false;
    }

    const EGLint attribute[] = {EGL_NONE};
    mSurface = eglCreateWindowSurface(mEglCore->getDisplay(), mEglCore->getEglConfig(),
                                      mWindow, attribute);
    EGLint ret = eglGetError();
    if (ret != EGL_SUCCESS) {
        LogE("WindowSurface(%s), fail to prepare, err code = 0x%x", mName.c_str(), ret);
        mSurface = EGL_NO_SURFACE;
    } else {
        mWidth = ANativeWindow_getWidth(mWindow);
        mHeight = ANativeWindow_getHeight(mWindow);
        LogI("WindowSurface(%s) success, width(%d) height(%d)", mName.c_str(), mWidth, mHeight);
    }
    return ret == EGL_SUCCESS;
}

void WindowSurface::release() {
    if (mSurface != EGL_NO_SURFACE) {
        eglMakeCurrent(mEglCore->getDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglDestroySurface(mEglCore->getDisplay(), mSurface)) {
            LogI("WindowSurface(%s) destroy surface", mName.c_str());
        } else {
            LogE("WindowSurface(%s) fails to destroy surface, err code = 0x%x",
                 mName.c_str(), eglGetError());
        }
        mEglCore.reset();
        mSurface = EGL_NO_SURFACE;
    }
    if (mWindow != nullptr) {
        LogI("WindowSurface(%s) release native window", mName.c_str());
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
}

bool WindowSurface::swapBuffer() {
    bool success{false};
    if (mEglCore == nullptr) {
        LogE("WindowSurface(%s) EglCore is null", mName.c_str());
        goto done;
    }
    if (mSurface == EGL_NO_SURFACE) {
        LogE("WindowSurface(%s) surface is null", mName.c_str());
        goto done;
    }
    if (mEglCore->getDisplay() == EGL_NO_DISPLAY) {
        LogE("WindowSurface(%s) display is null", mName.c_str());
        goto done;
    }
    if (!eglSwapBuffers(mEglCore->getDisplay(), mSurface)) {
        LogE("WindowSurface(%s) fail to swap buffer, err code = 0x%x", mName.c_str(), eglGetError());
        goto done;
    }
    success = true;
    done:
    return success;
}

