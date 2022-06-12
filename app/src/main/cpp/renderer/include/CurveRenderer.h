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
    /**
     * draw smooth bessel quadratic curve
     * @param startPt start point of bessel
     * @param controlPt control point of bessel
     * @param endPt end point of bessel
     */
    void drawCurve(const Point2d &startPt, const Point2d &controlPt, const Point2d &endPt, uint32_t lineWidth);
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
    unsigned int mStartHandler;
    unsigned int mControlHandler;
    unsigned int mEndHandler;
    unsigned int mLineWidthHandler;

    unsigned int mVbo[1];
};

#endif //OPENGLASSEMBLE_CURVERENDERER_H
