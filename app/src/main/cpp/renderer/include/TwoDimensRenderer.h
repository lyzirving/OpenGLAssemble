//
// Created by liuyuzhou on 2022/5/23.
//
#ifndef OPENGLASSEMBLE_TWODIMENSRENDERER_H
#define OPENGLASSEMBLE_TWODIMENSRENDERER_H

#include "BaseRendererProgram.h"

class TwoDimensRenderer : public BaseRendererProgram {
public:
    TwoDimensRenderer(const char *name);
    ~TwoDimensRenderer();

    virtual void draw(unsigned int textureId) override;
    virtual void release() override;
    virtual void updateVertexCoordinate(uint32_t viewX, uint32_t viewY, uint32_t width, uint32_t height);
    virtual void updateTextureCoordinate(uint32_t startX, uint32_t startY, uint32_t subWidth, uint32_t subHeight,
            uint32_t rawWidth, uint32_t rawHeight, bool flipX = false, bool flipY = false);
protected:
    virtual void flushVertexCoordinate();
    virtual void flushTextureCoordinate();

    virtual bool initProgram() override;
    virtual void initHandler() override;
    virtual void initCoordinate() override;
    virtual void initBuffer() override;

    unsigned int mVertexHandler;
    unsigned int mTexCoordHandler;
    unsigned int mTwoDimenSampler;
    unsigned int mMatrixHandler;

    float mVertex[8];
    float mTexCoordinate[8];
    unsigned int mVbo[2];
private:
};

#endif //OPENGLASSEMBLE_TWODIMENSRENDERER_H
