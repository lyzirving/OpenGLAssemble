//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_RENDERERCONTEXT_H
#define OPENGLASSEMBLE_RENDERERCONTEXT_H

#include <pthread.h>
#include <unordered_map>

#include "SimpleLooper.h"

struct ANativeWindow;
class EglCore;
class WindowSurface;
class TwoDimensRenderer;
class GraphicRenderer;
class AntialiasLineRenderer;
class ContinuousLineRenderer;

enum MessageId : uint8_t {
    MESSAGE_QUIT = 0x00,
    MESSAGE_REGISTER_WINDOW,
    MESSAGE_REMOVE_WINDOW,
    MESSAGE_REQUEST_DRAW
};

class RendererContext {
public:
    RendererContext(const char* name);

    virtual ~RendererContext();
    virtual void draw();

    void handleRegisterWindow(const char *name);
    void prepareAndLoop();
    void quitLoop();
    void requestQuit();
    bool registerWindow(const char* name, ANativeWindow *window);
    void removeWindow(const char* name);
    void sendMessage(uint32_t what, uint32_t arg0 = 0, uint32_t arg1 = 0, const char* argStr = nullptr);

protected:
    virtual bool onPrepare();
    virtual void onQuit();

    pthread_t mThreadId;
    SimpleLooper mLooper;
    std::shared_ptr<EglCore> mEglCore;
    std::unordered_map<std::string, std::shared_ptr<WindowSurface>> mWindows;

private:
    std::shared_ptr<TwoDimensRenderer> mTwoDimensRenderer;
    std::shared_ptr<GraphicRenderer> mGraphicRenderer;
    std::shared_ptr<AntialiasLineRenderer> mAntialiasLineRenderer;
    std::shared_ptr<ContinuousLineRenderer> mContinuousLineRenderer;
};

class RendererHandler : public MessageHandler {
public:
    RendererHandler(RendererContext *ctx, uint32_t arg0 = 0, uint32_t arg1 = 0, const char* argStr = nullptr);
    ~RendererHandler();
    virtual void handleMessage(uint8_t what) override;

    RendererContext *mCtx;
    uint32_t mIntArg0;
    uint32_t mIntArg1;
    std::string mStrArg;
};

#endif //OPENGLASSEMBLE_RENDERERCONTEXT_H
