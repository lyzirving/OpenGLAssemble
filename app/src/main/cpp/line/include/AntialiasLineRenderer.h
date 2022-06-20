//
// Created by lyzirving on 2022/5/24.
//
#ifndef OPENGLASSEMBLE_ANTIALIASLINERENDERER_H
#define OPENGLASSEMBLE_ANTIALIASLINERENDERER_H

#include "BaseRendererProgram.h"

class Point2d;

class AntialiasLineRenderer : public BaseRendererProgram {
public:
    AntialiasLineRenderer(const char *name);
    ~AntialiasLineRenderer();

    /**
     * create a polygon for OpenGL ES to draw smooth line
     * two points for a line will generate four vertex to be renderer as a rectangle
     * @param startPt     start point(x, y) of the line in screen coordinate
     * @param endPt     end point(x, y) of the line in screen coordinate
     * @param lineWidth  line width in screen coordinate unit
     * @param color      color to be fill into the line, it should be a form like 0xffffffff which represents for RGBA
     */
    void drawSegment(const Point2d &startPt, const Point2d &endPt, uint32_t lineWidth, uint32_t color = 0xffffffff);
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

#endif //OPENGLASSEMBLE_ANTIALIASLINERENDERER_H
