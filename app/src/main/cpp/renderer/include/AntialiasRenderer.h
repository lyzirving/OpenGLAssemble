//
// Created by lyzirving on 2022/5/24.
//
#ifndef OPENGLASSEMBLE_ANTIALIASRENDERER_H
#define OPENGLASSEMBLE_ANTIALIASRENDERER_H

#include "BaseRendererProgram.h"

class AntialiasRenderer : public BaseRendererProgram {
public:
    AntialiasRenderer(const char *name);
    ~AntialiasRenderer();

    /**
     * create a polygon for OpenGL ES to draw smooth line
     * two points for a line will generate four vertex to be renderer as a rectangle
     * @param point1     start point(x, y) of the line in screen coordinate
     * @param point2     end point(x, y) of the line in screen coordinate
     * @param lineWidth  line width in screen coordinate unit, this param will be divided by the length of screen diagonal
     * @param color      color to be fill into the line, it should be a form like 0xffffffff which represents for RGBA
     */
    void drawSegment(uint32_t *point1, uint32_t *point2, float lineWidth, uint32_t color = 0xffffffff);
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

    unsigned int mVbo[1];
};

#endif //OPENGLASSEMBLE_ANTIALIASRENDERER_H
