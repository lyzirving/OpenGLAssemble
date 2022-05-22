//
// Created by lyzirving on 2022/5/22.
//
#include <cstring>
#include <GLES2/gl2.h>

#include "BaseRendererProgram.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BaseRendererProgram"

BaseRendererProgram::BaseRendererProgram(const char *name, const char *vertexShader,
                                         const char *fragShader)
        : mInitialized(false), mName(name), mProgram(0) {
    std::memset(mMatrix, 0, sizeof(float) * 16);
    if(vertexShader != nullptr && std::strlen(vertexShader) != 0) {
        mVertexShader = static_cast<char *>(std::calloc(strlen(vertexShader + 1), sizeof(char)));
        std::memcpy(mVertexShader, vertexShader, strlen(vertexShader) * sizeof(char));
    } else {
        LogE("input vertex shader is null");
    }
    if(fragShader != nullptr && std::strlen(fragShader) != 0) {
        mFragmentShader = static_cast<char *>(std::calloc(strlen(fragShader + 1), sizeof(char)));
        std::memcpy(mFragmentShader, fragShader, strlen(fragShader) * sizeof(char));
    } else {
        LogE("input fragment shader is null");
    }
}

BaseRendererProgram::~BaseRendererProgram() {
    if (mVertexShader != nullptr) {
        std::free(mVertexShader);
        mVertexShader = nullptr;
    }
    if (mFragmentShader != nullptr) {
        std::free(mFragmentShader);
        mFragmentShader = nullptr;
    }
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
    onPostInit(success);
    return success;
}

bool BaseRendererProgram::initProgram() {
    mProgram = GlHelper::buildProgram(mVertexShader, mFragmentShader);
    return mProgram != 0;
}

void BaseRendererProgram::release() {
    if(mProgram != 0) {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}
