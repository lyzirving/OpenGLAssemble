//
// Created by liuyuzhou on 2022/6/7.
//
#ifndef OPENGLASSEMBLE_CONTINUOUSLINERENDERER_H
#define OPENGLASSEMBLE_CONTINUOUSLINERENDERER_H

#include "BaseRendererProgram.h"

class Point2d;

class ContinuousLineRenderer : public BaseRendererProgram {
public:
    ContinuousLineRenderer(const char *name);
    ~ContinuousLineRenderer();

    void drawLines(Point2d *points, uint32_t count, uint32_t lineWidth, uint32_t color = 0xffffffff);
    virtual void release() override;

protected:
    virtual bool initProgram() override;
    virtual void initHandler() override;
    virtual void initCoordinate() override;
    virtual void initBuffer() override;

    virtual void onBeforeInit() override;
    virtual void onPostInit(bool success) override;

    unsigned int mVertexHandler;
    unsigned int mTexCoordHandler;
    unsigned int mColorHandler;
    unsigned int mThresholdHandler;
    //mThreshold will be limit from 0.35 to 0.5 in fragment shader
    float mThreshold;
    float mColor[4];
    unsigned int mVbo[2];
};

#endif //OPENGLASSEMBLE_CONTINUOUSLINERENDERER_H
