//
// Created by lyzirving on 2022/5/24.
//
#ifndef OPENGLASSEMBLE_ANTIALIASINGLINERENDERER_H
#define OPENGLASSEMBLE_ANTIALIASINGLINERENDERER_H

#include "BaseRendererProgram.h"

class AntialiasingLineRenderer : public BaseRendererProgram {
public:
    AntialiasingLineRenderer(const char *name);
    ~AntialiasingLineRenderer();

    void drawSegment(uint32_t *start, uint32_t *end, float lineWidth, uint32_t color = 0xffffffff);
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

#endif //OPENGLASSEMBLE_ANTIALIASINGLINERENDERER_H
