//
// Created by lyzirving on 2022/5/22.
//
#ifndef OPENGLASSEMBLE_BASERENDERERPROGRAM_H
#define OPENGLASSEMBLE_BASERENDERERPROGRAM_H

#include <string>

struct Viewport {
public:
    uint32_t mStartX;
    uint32_t mStartY;
    uint32_t mWidth;
    uint32_t mHeight;

    Viewport() : mStartX(0), mStartY(0), mWidth(0), mHeight(0) {}
    Viewport(uint32_t x, uint32_t y, uint32_t width, int32_t height)
            : mStartX(x), mStartY(y), mWidth(width), mHeight(height) {}
};

class BaseRendererProgram {
public:
    BaseRendererProgram(const char *name);
    virtual ~BaseRendererProgram();

    void calculateVertex(float *vertex, float x, float y);
    bool init();
    const Viewport &getViewport();
    void updateViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    virtual void release();
protected:

    virtual bool initProgram() = 0;
    virtual void initHandler() = 0;
    virtual void initCoordinate() = 0;
    virtual void initBuffer() = 0;

    virtual void onBeforeInit() = 0;
    virtual void onPostInit(bool success) = 0;

    bool mInitialized;
    std::string mName;
    unsigned int mProgram;
    unsigned int mMatrixHandler;
    float mMatrix[16];
    Viewport mViewport;
};

#endif //OPENGLASSEMBLE_BASERENDERERPROGRAM_H
