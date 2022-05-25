//
// Created by lyzirving on 2022/5/19.
//
#include <cstring>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "RendererContext.h"
#include "WindowSurface.h"
#include "TwoDimensRenderer.h"
#include "GraphicRenderer.h"
#include "AntialiasRenderer.h"
#include "RendererMetadata.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RendererContext"

void *threadStart(void *arg) {
    LogFunctionEnter;
    auto *context = static_cast<RendererContext *>(arg);
    context->prepareAndLoop();
    context->quitLoop();
    delete context;
    LogFunctionExit;
    return nullptr;
}

RendererContext::RendererContext(const char *name) : mThreadId(0), mLooper(name),
                                                     mEglCore(new EglCore),
                                                     mTwoDimensRenderer(new TwoDimensRenderer(renderer::TWO_DIMEN_RENDERER)),
                                                     mGraphicRenderer(new GraphicRenderer(renderer::GRAPHIC_RENDERER)),
                                                     mAntialiasRenderer(new AntialiasRenderer(renderer::ANTI_ALIAS_RENDERER)),
                                                     mWindows() {
    pthread_create(&mThreadId, nullptr, threadStart, this);
}

RendererContext::~RendererContext() {
    mTwoDimensRenderer.reset();
    mGraphicRenderer.reset();
    mAntialiasRenderer.reset();
    mEglCore.reset();
}

RendererHandler::RendererHandler(RendererContext *ctx, uint32_t arg0, uint32_t arg1, const char* argStr)
        : mCtx(ctx), mIntArg0(arg0), mIntArg1(arg1), mStrArg(argStr != nullptr ? argStr : "") {}

RendererHandler::~RendererHandler() {
    //only set the context pointer null, do not deconstruct it
    mCtx = nullptr;
}

void RendererHandler::handleMessage(uint8_t what) {
    switch (what) {
        case MessageId::MESSAGE_QUIT: {
            mCtx->requestQuit();
            break;
        }
        case MessageId::MESSAGE_REGISTER_WINDOW: {
            mCtx->handleRegisterWindow(mStrArg.c_str());
            break;
        }
        case MessageId::MESSAGE_REMOVE_WINDOW: {
            mCtx->removeWindow(mStrArg.c_str());
            break;
        }
        case MessageId::MESSAGE_REQUEST_DRAW: {
            mCtx->draw();
            break;
        }
        default: {
            LogI("handle unknown message(%d)", what);
            break;
        }
    }
}

void RendererContext::draw() {
    auto it = mWindows.begin();
    while(it != mWindows.end()) {
        std::shared_ptr<WindowSurface> window = it->second;
        window->makeCurrent();
        glClearColor(float(1), float(1), float(1), float(1));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        uint32_t width = window->getWidth();
        uint32_t height = window->getHeight();

        glViewport(0, 0, width, height);
        mAntialiasRenderer->updateViewport(0, 0, width, height);
        uint32_t start[2];
        uint32_t end[2];

        float pt1X = float(width) / 2.f + float(width) / 6.f;
        float pt1Y = float(height) / 5.f;
        float pt2X = float(width) / 4.f;
        float pt2Y = float(height) / 3.f;

        start[0] = pt1X;
        start[1] = pt1Y;
        end[0] = pt2X;
        end[1] = pt2Y;
        mAntialiasRenderer->drawSegment(start, end, 0.05, 0xf26522ff);

        mGraphicRenderer->updateViewport(0, 0, width, height);
        float vArray[4];
        VectorHelper::vertex2d(vArray, pt1X, pt1Y, mGraphicRenderer->getViewport());
        VectorHelper::vertex2d(vArray + 2, pt2X, pt2Y, mGraphicRenderer->getViewport());
        mGraphicRenderer->drawGradientLines(vArray, 2, 2,
                                            0x6950a1ff, 0xf26522ff, 10);
        window->swapBuffer();
        it++;
    }
}

void RendererContext::handleRegisterWindow(const char *name) {
    std::string key(name);
    auto it = mWindows.find(key);
    if(it != mWindows.end()) {
        it->second->prepare();
    }
}

void RendererContext::prepareAndLoop() {
    if (!mLooper.isValid()) {
        LogE("looper(%s) is not valid", mLooper.getName().c_str());
        goto done;
    }
    if(!mEglCore->prepare())
        goto done;

    if(!mTwoDimensRenderer->init())
        goto done;

    if(!mGraphicRenderer->init())
        goto done;

    if(!mAntialiasRenderer->init())
        goto done;

    mLooper.loop();

    done:
    LogFunctionExit;
}

void RendererContext::quitLoop() {
    auto it = mWindows.begin();
    while(it != mWindows.end()) {
        std::shared_ptr<WindowSurface> window = it->second;
        it = mWindows.erase(it);
        window.reset();
    }
    mTwoDimensRenderer->release();
    mGraphicRenderer->release();
    mAntialiasRenderer->release();
}

void RendererContext::sendMessage(uint32_t what, uint32_t arg0, uint32_t arg1, const char* argStr) {
    if (mLooper.isValid()) {
        std::shared_ptr<RendererHandler> handler(new RendererHandler(this, arg0, arg1, argStr));
        std::shared_ptr<Message> message(new Message(what, handler));
        mLooper.sendMessage(message);
    } else {
        LogE("looper(%s) is not valid", mLooper.getName().c_str());
    }
}

void RendererContext::requestQuit() {
    mLooper.requestQuit();
}

bool RendererContext::registerWindow(const char *name, ANativeWindow *window) {
    if(mEglCore == nullptr) {
        LogE("egl core is null");
        return false;
    }
    if(name == nullptr || strlen(name) == 0) {
        LogE("name is null");
        return false;
    }
    if(window == nullptr) {
        LogE("input window is null");
        return false;
    }
    std::string key(name);
    auto it = mWindows.find(key);
    if(it == mWindows.end()) {
        std::shared_ptr<WindowSurface> unit(new WindowSurface(mEglCore, window, name));
        mWindows.insert({key, unit});
        sendMessage(MessageId::MESSAGE_REGISTER_WINDOW, 0, 0, name);
    } else {
        LogI("window(%s) is already registered", name);
    }
    return true;
}

void RendererContext::removeWindow(const char *name) {
    std::string key(name);
    auto it = mWindows.find(key);
    if(it != mWindows.end()) {
        std::shared_ptr<WindowSurface> surface = it->second;
        mWindows.erase(it);
        surface.reset();
    }
}


