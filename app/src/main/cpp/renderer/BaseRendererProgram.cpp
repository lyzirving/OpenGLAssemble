//
// Created by lyzirving on 2022/5/22.
//
#include <cstring>
#include <GLES2/gl2.h>

#include "BaseRendererProgram.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BaseRendererProgram"

BaseRendererProgram::BaseRendererProgram(const char *name)
        : mInitialized(false),
          mName(name),
          mProgram(0),
          mMatrixHandler(0),
          mMatrix(),
          mViewport() {
    MatrixUtil::identity(mMatrix);
}

BaseRendererProgram::~BaseRendererProgram() = default;

void BaseRendererProgram::calculateVertex(float *vertex, float x, float y) {
    if (mViewport.mWidth == 0 || mViewport.mHeight == 0) {
        LogE("render(%s) invalid view port(%u, %u, %u, %u)", mName.c_str(), mViewport.mStartX,
             mViewport.mStartY, mViewport.mWidth, mViewport.mHeight);
        return;
    }
    float halfPortWidth = ((float) mViewport.mWidth) / ((float) 2);
    float halfPortHeight = ((float) mViewport.mHeight) / ((float) 2);
    *vertex = (x - halfPortWidth) / halfPortWidth;
    vertex++;
    *vertex = (halfPortHeight - y) / halfPortHeight;
}

const Viewport &BaseRendererProgram::getViewport() {
    return mViewport;
}

bool BaseRendererProgram::init() {
    onBeforeInit();
    bool success = initProgram();
    if(!success)
        goto done;

    initHandler();
    success = GlHelper::checkGlError("fail to get handler");;
    if(!success)
        goto done;

    initCoordinate();
    success = GlHelper::checkGlError("fail to init coordinate");
    if(!success)
        goto done;

    initBuffer();
    success = GlHelper::checkGlError("fail to init buffer");
    if(!success)
        goto done;

    done:
    if(!success)
        release();

    mInitialized = success;
    LogI("renderer(%s) succeed to init %s", mName.c_str(), (success ? "true" : "false"));

    onPostInit(success);
    return success;
}

void BaseRendererProgram::release() {
    if(mProgram != 0) {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

void BaseRendererProgram::updateViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    mViewport.mStartX = x;
    mViewport.mStartY = y;
    mViewport.mWidth = width;
    mViewport.mHeight = height;
}
