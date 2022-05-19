//
// Created by lyzirving on 2022/5/19.
//
#include "RendererContext.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RendererContext"

void *threadStart(void *arg) {
    LogFunctionEnter;
    auto *context = static_cast<RendererContext *>(arg);
    context->prepare();
    LogFunctionExit;
    return nullptr;
}

RendererContext::RendererContext(const char *name) : mLooper(name), mThreadId(0) {
    pthread_create(&mThreadId, nullptr, threadStart, this);
}

RendererContext::~RendererContext() = default;

RendererHandler::RendererHandler(RendererContext *ctx) : mCtx(ctx) {}

RendererHandler::~RendererHandler() {
    //only set the context pointer null, do not deconstruct it
    mCtx = nullptr;
}

void RendererHandler::handleMessage(const Message &message) {

}

void RendererContext::prepare() {
    mLooper.loop();
}


