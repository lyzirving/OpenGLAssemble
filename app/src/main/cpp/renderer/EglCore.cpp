//
// Created by lyzirving on 2022/5/21.
//
#include <GLES2/gl2.h>

#include "EglCore.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EglCore"

EglCore::EglCore() : mEglDisplay(EGL_NO_DISPLAY),
                     mEglCtx(EGL_NO_CONTEXT),
                     mEglConfig(EGL_NO_CONFIG_KHR) {
}

EglCore::~EglCore() {
    release();
}

EGLContext EglCore::getEglContext() {
    return mEglCtx;
}

EGLDisplay EglCore::getDisplay() {
    return mEglDisplay;
}

EGLConfig EglCore::getEglConfig() {
    return mEglConfig;
}

bool EglCore::makeCurrent(EGLSurface draw, EGLSurface read) {
    bool success{false};
    if (mEglCtx != EGL_NO_CONTEXT) {
        unsigned int ret = eglMakeCurrent(mEglDisplay, draw, read, mEglCtx);
        if (ret != EGL_SUCCESS) {
            LogE("fail to make current, err code = 0x%x", eglGetError());
        }
    } else {
        LogE("egl context is null");
    }
    return success;
}

void EglCore::release() {
    if (mEglCtx != EGL_NO_CONTEXT) {
        LogI("destroy egl context");
        eglDestroyContext(mEglDisplay, mEglCtx);
    }
    if (mEglDisplay != EGL_NO_DISPLAY) {
        LogI("terminate egl display");
        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglTerminate(mEglDisplay);
    }
    mEglCtx = EGL_NO_CONTEXT;
    mEglDisplay = EGL_NO_DISPLAY;
    mEglConfig = EGL_NO_CONFIG_KHR;
}

bool EglCore::prepare(EGLContext shareContext) {
    bool success{false};
    EGLConfig eglConfig{nullptr};
    EGLint major = 0;
    EGLint minor = 0;
    EGLint numConfig;
    EGLint version;

    EGLDisplay display{EGL_NO_DISPLAY};
    EGLContext context{EGL_NO_CONTEXT};

    // hardcoded to RGBx output display
    const EGLint configAttribute[] = {
            // tag                  value
            // EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT | EGL_OPENGL_ES3_BIT_KHR,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_RECORDABLE_ANDROID, EGL_TRUE,
            EGL_NONE};

    // select OpenGL ES v3
    const EGLint contextAttribute[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    if (shareContext != EGL_NO_CONTEXT) {
        LogI("prepare for share context");
    }

    if (mEglCtx != EGL_NO_CONTEXT) {
        LogI("already prepared");
        success = true;
        goto quit;
    }
    // set up our OpenGL ES context associated with the default display
    // the display won't be visible
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LogE("failed to get display, reason = 0x%x", eglGetError());
        goto quit;
    }

    if (!eglInitialize(display, &major, &minor)) {
        LogE("failed to initialize display, reason = 0x%x", eglGetError());
        goto quit;
    }
    LogI("initialize display: version(%d.%d) vendor(%s)",
         major, minor, eglQueryString(display, EGL_VENDOR));

    // select the configuration that matches our desired characteristics best
    if (!eglChooseConfig(display, configAttribute, &eglConfig, 1, &numConfig)) {
        LogE("failed to get desired config, reason = 0x%x", eglGetError());
        goto quit;
    }

    // create egl context
    context = eglCreateContext(display, eglConfig, shareContext, contextAttribute);
    if (context == EGL_NO_CONTEXT) {
        LogE("failed to create egl context, reason = 0x%x", eglGetError());
        goto quit;
    }

    eglQueryContext(display, context, EGL_CONTEXT_CLIENT_VERSION, &version);
    LogI("succeed to prepare egl environment, information is list as bellow:\n"
         "****** EGL context client version %d\n"
         "****** OpenGL ES version          %s\n"
         "****** vendor                     %s\n"
         "****** renderer                   %s\n"
         "****** max texture size           %s\n"
         "****** max viewport dims          %s",
         version, glGetString(GL_VERSION), glGetString(GL_VENDOR),
         glGetString(GL_RENDERER), glGetString(GL_MAX_TEXTURE_SIZE),
         glGetString(GL_MAX_VIEWPORT_DIMS));
    success = true;

    goto quit;

    quit:
    if (!success) {
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (display != EGL_NO_DISPLAY) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            eglTerminate(display);
        }
    } else {
        mEglDisplay = display;
        mEglCtx = context;
        mEglConfig = eglConfig;
    }
    return success;
}

void EglCore::swapBuffer(bool block) {
    if (mEglCtx != EGL_NO_CONTEXT) {
        if (block) {
            glFinish();
        } else {
            glFlush();
        }
    } else {
        LogE("egl context is null");
    }
}



