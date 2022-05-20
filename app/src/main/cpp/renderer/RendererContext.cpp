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

RendererContext::RendererContext(const char *name) : mLooper(name, this), mThreadId(0) {
    pthread_create(&mThreadId, nullptr, threadStart, this);
}

RendererContext::~RendererContext() = default;

RendererHandler::RendererHandler(RendererContext *ctx) : mCtx(ctx) {}

RendererHandler::~RendererHandler() {
    //only set the context pointer null, do not deconstruct it
    mCtx = nullptr;
    LogI("deconstruct");
}

void RendererHandler::handleMessage(const Message &message) {
    switch(message.what) {
        case MessageId::MESSAGE_QUIT: {
            mCtx->requestQuit();
            break;
        }
        default: {
            LogI("handle unknown message(%d)", message.what);
            break;
        }
    }
}

void RendererContext::onLooperQuit() {

}

void RendererContext::prepare() {
    if(mLooper.isValid()) {
        mLooper.loop();
    } else {
        LogE("looper(%s) is not valid", mLooper.getName().c_str());
    }
}

void RendererContext::sendMessage(uint32_t what) {
    if(mLooper.isValid()) {
        std::shared_ptr<RendererHandler> handler(new RendererHandler(this));
        Message msg(what);
        mLooper.sendMessage(handler, msg);
    } else {
        LogE("looper(%s) is not valid", mLooper.getName().c_str());
    }
}

void RendererContext::requestQuit() {
    mLooper.requestQuit();
}


