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
    float start[2];
    float end[2];
    calculateVertex(start, point1[0], point1[1]);
    calculateVertex(end, point2[0], point2[1]);
    float segVec[2];
    //after calculateVertex(), the segVec is normalized
    segVec[0] = end[0] - start[0];
    segVec[1] = end[1] - start[1];
    float slope;
    if(segVec[0] == 0) {//segment is parallel to y axis
        slope = 0;
    } else if(segVec[1] == 0) {//segment is parallel to x axis
        slope = -1;
    } else {
        slope = -1.f * segVec[0] / segVec[1];
    }
    float vertex[8];
    float startPt[2];
    float lastPt[2];
    if(slope == 0) {// normal is parallel to x axis
        if(start[1] < end[1]) {
            startPt[0] = start[0];
            startPt[1] = start[1];
            lastPt[0] = end[0];
            lastPt[1] = end[1];
        } else {
            startPt[0] = end[0];
            startPt[1] = end[1];
            lastPt[0] = start[0];
            lastPt[1] = start[1];
        }
        vertex[0] = startPt[0] - lineWidth;
        vertex[1] = startPt[1];

        vertex[2] = startPt[0] + lineWidth;
        vertex[3] = startPt[1];

        vertex[4] = lastPt[0] - lineWidth;
        vertex[5] = lastPt[1];

        vertex[6] = lastPt[0] + lineWidth;
        vertex[7] = lastPt[1];
    } else if(slope == -1) {// normal is parallel to y axis
        if(start[0] < end[0]) {
            startPt[0] = start[0];
            startPt[1] = start[1];
            lastPt[0] = end[0];
            lastPt[1] = end[1];
        } else {
            startPt[0] = end[0];
            startPt[1] = end[1];
            lastPt[0] = start[0];
            lastPt[1] = start[1];
        }
        vertex[0] = startPt[0];
        vertex[1] = startPt[1] + lineWidth;

        vertex[2] = startPt[0];
        vertex[3] = startPt[1] - lineWidth;

        vertex[4] = lastPt[0];
        vertex[5] = lastPt[1] + lineWidth;

        vertex[6] = lastPt[0];
        vertex[7] = lastPt[1] - lineWidth;
    } else if(slope < 0) {
        if(start[0] < end[0]) {
            startPt[0] = start[0];
            startPt[1] = start[1];
            lastPt[0] = end[0];
            lastPt[1] = end[1];
        } else {
            startPt[0] = end[0];
            startPt[1] = end[1];
            lastPt[0] = start[0];
            lastPt[1] = start[1];
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
        if(start[0] < end[0]) {
            startPt[0] = start[0];
            startPt[1] = start[1];
            lastPt[0] = end[0];
            lastPt[1] = end[1];
        } else {
            startPt[0] = end[0];
            startPt[1] = end[1];
            lastPt[0] = start[0];
            lastPt[1] = start[1];
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

