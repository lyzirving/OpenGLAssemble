//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_RENDERERCONTEXT_H
#define OPENGLASSEMBLE_RENDERERCONTEXT_H

#include <pthread.h>

#include "SimpleLooper.h"

class RendererContext {
public:
    RendererContext(const char* name);
    ~RendererContext();

    void prepare();

private:

    pthread_t mThreadId;
    SimpleLooper mLooper;
};

class RendererHandler : public MessageHandler {
public:
    RendererHandler(RendererContext *ctx);
    ~RendererHandler();
    virtual void handleMessage(const Message &message) override;

    RendererContext *mCtx;
};

#endif //OPENGLASSEMBLE_RENDERERCONTEXT_H
