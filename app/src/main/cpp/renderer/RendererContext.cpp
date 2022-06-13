//
// Created by lyzirving on 2022/5/19.
//
#include <cstring>
#include <memory>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "RendererContext.h"
#include "WindowSurface.h"
#include "TwoDimensRenderer.h"
#include "GraphicRenderer.h"
#include "AntialiasLineRenderer.h"
#include "ContinuousLineRenderer.h"
#include "CurveRenderer.h"
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
                                                     mWindows(),
                                                     mTwoDimensRenderer(nullptr),
                                                     mGraphicRenderer(nullptr),
                                                     mAntialiasLineRenderer(nullptr),
                                                     mContinuousLineRenderer(nullptr),
                                                     mCurveRenderer(nullptr) {
    pthread_create(&mThreadId, nullptr, threadStart, this);
}

RendererContext::~RendererContext() {
    mTwoDimensRenderer.reset();
    mGraphicRenderer.reset();
    mAntialiasLineRenderer.reset();
    mContinuousLineRenderer.reset();
    mCurveRenderer.reset();
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
        mCurveRenderer->updateViewport(0, 0, width, height);
        //mContinuousLineRenderer->updateViewport(0, 0, width, height);
        //mAntialiasLineRenderer->updateViewport(0, 0, width, height);

        Point2d lines[3];
        lines[0].mX = float(width) / 2.f - float(width) / 5.f;
        lines[0].mY = float(height) / 2.f - float(height) / 6.f;
        lines[1].mX = float(width) / 2.f - float(width) / 3.f;
        lines[1].mY = float(height) / 2.f + float(height) / 8.f;
        lines[2].mX = float(width) / 2.f + float(width) / 8.f;
        lines[2].mY = float(height) / 2.f + float(height) / 4.f;

        //mCurveRenderer->drawLine(lines[0], lines[2], 20, 0xff0000ff);
        mCurveRenderer->drawCurve(lines[0], lines[1], lines[2], 30, 0xff0000ff);
//        mContinuousLineRenderer->drawLines(lines, 4, 40, 0xf26522ff);
//        lines[3].mX = float(width) / 2.f + float(width) / 3.f - float(width) / 7.f;
//        lines[3].mY = float(height) / 2.f + float(height) / 4.f + float(height) / 8.f;

//        mAntialiasLineRenderer->drawLines(lines, 4, 40, 0xf26522ff);
//        mAntialiasLineRenderer->drawSegment(lines[0], lines[1], 40, 0xf26522ff);
//        mAntialiasLineRenderer->drawSegment(lines[1], lines[2], 40, 0xf26522ff);
//        mAntialiasLineRenderer->drawSegment(lines[2], lines[3], 40, 0xf26522ff);

//        float vArray[4];
//        mGraphicRenderer->updateViewport(0, 0, width, height);
//        VectorHelper::vertex2d(vArray, lines[0].mX, lines[0].mY, mGraphicRenderer->getViewport());
//        VectorHelper::vertex2d(vArray + 2, lines[1].mX, lines[1].mY, mGraphicRenderer->getViewport());
//        mGraphicRenderer->drawLines(vArray, 2, 2, 0x000000ff, 10);
//
//        mGraphicRenderer->updateViewport(0, 0, width, height);
//        VectorHelper::vertex2d(vArray, lines[1].mX, lines[1].mY, mGraphicRenderer->getViewport());
//        VectorHelper::vertex2d(vArray + 2, lines[2].mX, lines[2].mY, mGraphicRenderer->getViewport());
//        mGraphicRenderer->drawLines(vArray, 2, 2, 0x000000ff, 10);

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

bool RendererContext::onPrepare() {
    mTwoDimensRenderer = std::make_shared<TwoDimensRenderer>(renderer::TWO_DIMEN_RENDERER);
    if (!mTwoDimensRenderer->init())
        goto fail;

    mGraphicRenderer = std::make_shared<GraphicRenderer>(renderer::GRAPHIC_RENDERER);
    if (!mGraphicRenderer->init())
        goto fail;

    mAntialiasLineRenderer = std::make_shared<AntialiasLineRenderer>(renderer::ANTI_ALIAS_RENDERER);
    if (!mAntialiasLineRenderer->init())
        goto fail;

    mContinuousLineRenderer = std::make_shared<ContinuousLineRenderer>(renderer::CONTINUOUS_LINE_RENDERER);
    if (!mContinuousLineRenderer->init())
        goto fail;

    mCurveRenderer = std::make_shared<CurveRenderer>(renderer::CURVE_RENDERER);
    if (!mCurveRenderer->init())
        goto fail;

    return true;

    fail:
    return false;
}

void RendererContext::onQuit() {
    if(mTwoDimensRenderer)
        mTwoDimensRenderer->release();
    if (mGraphicRenderer)
        mGraphicRenderer->release();
    if(mAntialiasLineRenderer)
        mAntialiasLineRenderer->release();
    if (mContinuousLineRenderer)
        mContinuousLineRenderer->release();
    if (mCurveRenderer)
        mCurveRenderer->release();
}

void RendererContext::prepareAndLoop() {
    if (!mLooper.isValid()) {
        LogE("looper(%s) is not valid", mLooper.getName().c_str());
        goto done;
    }
    if(!mEglCore->prepare())
        goto done;

    if(!onPrepare())
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
    onQuit();
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


