//
// Created by lyzirving on 2022/6/12.
//
#ifndef OPENGLASSEMBLE_DOTRENDERER_H
#define OPENGLASSEMBLE_DOTRENDERER_H

#include "BaseRendererProgram.h"

class Point2d;

class DotRenderer : public BaseRendererProgram {
public:
    DotRenderer(const char *name);
    ~DotRenderer();

    void drawDot(Point2d *pts, uint32_t count, float ptSize, uint32_t color = 0x000000ff);
    virtual void release() override;

protected:
    virtual bool initProgram() override;
    virtual void initHandler() override;
    virtual void initCoordinate() override;
    virtual void initBuffer() override;

    virtual void onBeforeInit() override;
    virtual void onPostInit(bool success) override;

    unsigned int mVaHandler;
    unsigned int mColorHandler;
    unsigned int mResolutionHandler;
    unsigned int mPtSizeHandler;

    unsigned int mVbo[1];
};

#endif //OPENGLASSEMBLE_DOTRENDERER_H
