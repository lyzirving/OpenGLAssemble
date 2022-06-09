//
// Created by lyzirving on 2022/6/9.
//
#ifndef OPENGLASSEMBLE_CURVERENDERER_H
#define OPENGLASSEMBLE_CURVERENDERER_H

#include "BaseRendererProgram.h"

class Point2d;

class CurveRenderer : public BaseRendererProgram {
public:
    CurveRenderer(const char *name);
    ~CurveRenderer();

    void drawQuadraticCurve(const Point2d &startPt, const Point2d &midPt, const Point2d &endPt);
    virtual void release() override;

protected:
    virtual bool initProgram() override;
    virtual void initHandler() override;
    virtual void initCoordinate() override;
    virtual void initBuffer() override;

    virtual void onBeforeInit() override;
    virtual void onPostInit(bool success) override;

    unsigned int mVertexHandler;
    unsigned int mColorHandler;
    unsigned int mPointSizeHandler;

    unsigned int mVbo[1];
};

#endif //OPENGLASSEMBLE_CURVERENDERER_H
