//
// Created by lyzirving on 2022/5/23.
//
#ifndef OPENGLASSEMBLE_GRAPHICRENDERER_H
#define OPENGLASSEMBLE_GRAPHICRENDERER_H

#include "BaseRendererProgram.h"

class GraphicRenderer : public BaseRendererProgram {
public:
    GraphicRenderer(const char *name);
    ~GraphicRenderer();

    void drawLines(float *vertex, uint32_t vertexCnt, uint32_t vertexComponent,
                   uint32_t color = 0xffffffff, float lineWidth = 1);
    void drawGradientLines(float *vertex, uint32_t vertexCnt, uint32_t vertexComponent,
                           uint32_t startColor = 0xfaa755ff, uint32_t endColor = 0xd93a49ff,
                           float lineWidth = 1);
    virtual void release() override;

protected:
    virtual bool initProgram() override;
    virtual void initHandler() override;
    virtual void initCoordinate() override;
    virtual void initBuffer() override;

    virtual void onBeforeInit() override;
    virtual void onPostInit(bool success) override;

    unsigned int mVertexHandler;
    unsigned int mVertexCntHandler;
    unsigned int mGradientHandler;

    unsigned int mLastVertexCount;
    unsigned int mVbo[1];
    float mGradientColor[8];
};

#endif //OPENGLASSEMBLE_GRAPHICRENDERER_H
