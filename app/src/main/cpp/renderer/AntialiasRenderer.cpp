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
          mColorHandler(0),
          mVbo() {}

AntialiasRenderer::~AntialiasRenderer() = default;

void AntialiasRenderer::drawSegment(uint32_t *point1, uint32_t *point2, float lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float viewDiagonal = std::sqrt(mViewport.mWidth * mViewport.mWidth + mViewport.mHeight * mViewport.mHeight);
    float start[2], end[2], polygon[8];
    VectorHelper::vertex2d(start, point1[0], point1[1], mViewport);
    VectorHelper::vertex2d(end, point2[0], point2[1], mViewport);
    VectorHelper::segmentToPolygon(polygon, start, end, lineWidth / viewDiagonal);

    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), polygon, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glDrawArrays(GL_POINTS, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

bool AntialiasRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::POINT_VERTEX_SHADER, shader::POINT_FRAGMENT_SHADER);
    return mProgram != 0;
}

void AntialiasRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
//    mColorHandler = glGetUniformLocation(mProgram, "uColor");
//    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
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

