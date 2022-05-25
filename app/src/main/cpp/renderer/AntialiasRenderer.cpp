//
// Created by lyzirving on 2022/5/24.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cmath>

#include "AntialiasRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AntialiasRenderer"

AntialiasRenderer::AntialiasRenderer(const char *name)
        : BaseRendererProgram(name),
          mVertexHandler(0),
          mLineHandler(0),
          mThresholdHandler(0),
          mColorHandler(0),
          mPortSizeHandler(0),
          mHalfLineWidthHandler(0),
          mColor(),
          mLines(),
          mVbo() {}

AntialiasRenderer::~AntialiasRenderer() = default;

void AntialiasRenderer::drawSegment(uint32_t *point1, uint32_t *point2, float lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float viewDiagonal = std::sqrt(mViewport.mWidth * mViewport.mWidth + mViewport.mHeight * mViewport.mHeight);
    float shaderLineWidth = lineWidth / viewDiagonal;
    float start[2], end[2], polygon[8];
    VectorHelper::vertex2d(start, point1[0], point1[1], mViewport);
    VectorHelper::vertex2d(end, point2[0], point2[1], mViewport);
    VectorHelper::segmentToPolygon(polygon, start, end, shaderLineWidth);

    glUseProgram(mProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), polygon, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);

    mColor[0] = (GLfloat)CHANNEL_R(color);
    mColor[1] = (GLfloat)CHANNEL_G(color);
    mColor[2] = (GLfloat)CHANNEL_B(color);
    mColor[3] = (GLfloat)CHANNEL_A(color);
    glUniform4fv(mColorHandler, 1, mColor);

    float threshold = shaderLineWidth / 2.f * 0.45f;
    glUniform1f(mThresholdHandler, threshold);

    mLines[0] = start[0];
    mLines[1] = start[1];
    mLines[2] = end[0];
    mLines[3] = end[1];
    glUniform2fv(mLineHandler, 2, mLines);
    glUniform1f(mHalfLineWidthHandler, shaderLineWidth / 2.f);
    glUniform2f(mPortSizeHandler, float(mViewport.mWidth), float(mViewport.mHeight));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

bool AntialiasRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::ANTI_ALIS_LINE_VERTEX_SHADER, shader::ANTI_ALIS_LINE_FRAGMENT_SHADER);
    return mProgram != 0;
}

void AntialiasRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mLineHandler = glGetUniformLocation(mProgram, "uLines");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
    mPortSizeHandler = glGetUniformLocation(mProgram, "uPortSize");
    mHalfLineWidthHandler = glGetUniformLocation(mProgram, "uHalfLineWidth");
}

void AntialiasRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void AntialiasRenderer::initBuffer() {
    //empty implementation for this case
}

void AntialiasRenderer::onBeforeInit() {}

void AntialiasRenderer::onPostInit(bool success) {}

void AntialiasRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

