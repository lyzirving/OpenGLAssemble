//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_RENDERERCONTEXT_H
#define OPENGLASSEMBLE_RENDERERCONTEXT_H

#include <pthread.h>

#include "SimpleLooper.h"
#include "EglCore.h"

enum MessageId : uint8_t {
    MESSAGE_QUIT = 0x00,
};

class RendererContext {
public:
    RendererContext(const char* name);
    ~RendererContext();

    void prepareAndLoop();
    void quitLoop();
    void requestQuit();
    void sendMessage(uint32_t what);

private:

    pthread_t mThreadId;
    SimpleLooper mLooper;
    EglCore mEglCore;
};

class RendererHandler : public MessageHandler {
public:
    RendererHandler(RendererContext *ctx);
    ~RendererHandler();
    virtual void handleMessage(const Message &message) override;

    RendererContext *mCtx;
};

#endif //OPENGLASSEMBLE_RENDERERCONTEXT_H
