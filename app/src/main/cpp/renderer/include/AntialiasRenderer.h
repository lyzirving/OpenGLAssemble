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
