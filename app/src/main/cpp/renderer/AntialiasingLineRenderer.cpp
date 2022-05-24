//
// Created by lyzirving on 2022/5/24.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cmath>

#include "AntialiasingLineRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AntialiasingLineRenderer"

AntialiasingLineRenderer::AntialiasingLineRenderer(const char *name)
        : BaseRendererProgram(name),
          mVertexHandler(0),
          mColorHandler(0),
          mVbo() {}

AntialiasingLineRenderer::~AntialiasingLineRenderer() = default;

void AntialiasingLineRenderer::drawSegment(uint32_t *start, uint32_t *end, float lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float first[2];
    float last[2];
    calculateVertex(first, start[0], start[1]);
    calculateVertex(last, end[0], end[1]);
    float segVec[2];
    //after calculateVertex(), the segVec is normalized
    segVec[0] = last[0] - first[0];
    segVec[1] = last[1] - first[1];
    float slope;
    if(segVec[0] == 0) {//segment is parallel to y axis
        slope = 0;
    } else if(segVec[1] == 0) {//segment is parallel to x axis
        slope = -1;
    } else {
        slope = -1.f * segVec[0] / segVec[1];
    }
    float vertex[8];
    if(slope == 0) {// normal is parallel to x axis
        vertex[0] = first[0] - lineWidth;
        vertex[1] = first[1];

        vertex[2] = first[0] + lineWidth;
        vertex[3] = first[1];

        vertex[4] = last[0] - lineWidth;
        vertex[5] = last[1];

        vertex[6] = last[0] + lineWidth;
        vertex[7] = last[1];
    } else if(slope == -1) {// normal is parallel to y axis
        vertex[0] = first[0];
        vertex[1] = first[1] + lineWidth;

        vertex[2] = first[0];
        vertex[3] = first[1] - lineWidth;

        vertex[4] = last[0];
        vertex[5] = last[1] + lineWidth;

        vertex[6] = last[0];
        vertex[7] = last[1] - lineWidth;
    } else if(slope < 0) {
        float startPt[2];
        float lastPt[2];
        if(first[0] < last[0]) {
            startPt[0] = first[0];
            startPt[1] = first[1];
            lastPt[0] = last[0];
            lastPt[1] = last[1];
        } else {
            startPt[0] = last[0];
            startPt[1] = last[1];
            lastPt[0] = first[0];
            lastPt[1] = first[1];
        }
        float angle = std::atan(-slope);
        vertex[0] = startPt[0] - lineWidth * std::cos(angle);
        vertex[1] = startPt[1] + lineWidth * std::sin(angle);

        vertex[2] = startPt[0] + lineWidth * std::cos(angle);
        vertex[3] = startPt[1] - lineWidth * std::sin(angle);

        vertex[4] = lastPt[0] - lineWidth * std::cos(angle);
        vertex[5] = lastPt[1] + lineWidth * std::sin(angle);

        vertex[6] = lastPt[0] + lineWidth * std::cos(angle);
        vertex[7] = lastPt[1] - lineWidth * std::sin(angle);
    } else {
        float startPt[2];
        float lastPt[2];
        if(first[0] < last[0]) {
            startPt[0] = first[0];
            startPt[1] = first[1];
            lastPt[0] = last[0];
            lastPt[1] = last[1];
        } else {
            startPt[0] = last[0];
            startPt[1] = last[1];
            lastPt[0] = first[0];
            lastPt[1] = first[1];
        }
        float angle = std::atan(slope);
        vertex[0] = startPt[0] + lineWidth * std::cos(angle);
        vertex[1] = startPt[1] + lineWidth * std::sin(angle);

        vertex[2] = startPt[0] - lineWidth * std::cos(angle);
        vertex[3] = startPt[1] - lineWidth * std::sin(angle);

        vertex[4] = lastPt[0] + lineWidth * std::cos(angle);
        vertex[5] = lastPt[1] + lineWidth * std::sin(angle);

        vertex[6] = lastPt[0] - lineWidth * std::cos(angle);
        vertex[7] = lastPt[1] - lineWidth * std::sin(angle);
    }

    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), vertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glDrawArrays(GL_POINTS, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

bool AntialiasingLineRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::POINT_VERTEX_SHADER, shader::POINT_FRAGMENT_SHADER);
    return mProgram != 0;
}

void AntialiasingLineRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
//    mColorHandler = glGetUniformLocation(mProgram, "uColor");
//    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void AntialiasingLineRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void AntialiasingLineRenderer::initBuffer() {
    //empty implementation for this case
}

void AntialiasingLineRenderer::onBeforeInit() {}

void AntialiasingLineRenderer::onPostInit(bool success) {}

void AntialiasingLineRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

