//
// Created by lyzirving on 2022/6/8.
//
#include <cstring>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "CurveRenderer.h"
#include "CurveRendererContext.h"
#include "WindowSurface.h"
#include "RendererMetadata.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CurveRendererContext"

CurveRendererContext::CurveRendererContext(const char *name)
        : RendererContext(name),
        mCurveRenderer(nullptr) {}

CurveRendererContext::~CurveRendererContext() {
    mCurveRenderer.reset();
}

void CurveRendererContext::draw() {
    auto it = mWindows.begin();
    while(it != mWindows.end()) {
        std::shared_ptr<WindowSurface> window = it->second;
        window->makeCurrent();
        glClearColor(float(1), float(1), float(1), float(1));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        uint32_t width = window->getWidth();
        uint32_t height = window->getHeight();
        glViewport(0, 0, width, height);

        window->swapBuffer();
        it++;
    }
}

bool CurveRendererContext::onPrepare() {
    mCurveRenderer = std::make_shared<CurveRenderer>(renderer::CURVE_RENDERER);
    if (!mCurveRenderer->init())
        goto fail;

    return true;

    fail:
    return false;
}

void CurveRendererContext::onQuit() {
    if (mCurveRenderer)
        mCurveRenderer->release();
}

