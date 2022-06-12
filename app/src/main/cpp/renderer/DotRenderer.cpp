//
// Created by lyzirving on 2022/6/12.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cmath>

#include "DotRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DotRenderer"

DotRenderer::DotRenderer(const char *name)
        : BaseRendererProgram(name),
          mVaHandler(0),
          mColorHandler(0),
          mResolutionHandler(0),
          mPtSizeHandler(0),
          mVbo(){}

DotRenderer::~DotRenderer() = default;

void DotRenderer::drawDot(Point2d *pts, uint32_t count, float ptSize, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float va[count * 2];
    for(uint32_t i = 0; i < count; i++) {
        va[i * 2 + 0] = pts[i].mX;
        va[i * 2 + 1] = pts[i].mY;
    }
    glUseProgram(mProgram);
    glEnableVertexAttribArray(mVaHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(GLfloat), va, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVaHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    glUniform4f(mColorHandler, CHANNEL_R(color), CHANNEL_G(color), CHANNEL_B(color), CHANNEL_A(color));
    glUniform1f(mPtSizeHandler, ptSize);
    glUniform2f(mResolutionHandler, mViewport.mWidth, mViewport.mHeight);

    glDrawArrays(GL_POINTS, 0, count);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVaHandler);
    glUseProgram(0);
}

bool DotRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/dot_vertex_shader.glsl"),
            GlHelper::readAssets("shader/dot_fragment_shader.glsl"));
    return mProgram != 0;
}

void DotRenderer::initHandler() {
    mVaHandler = glGetAttribLocation(mProgram, "aVertexAttribute");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mResolutionHandler = glGetUniformLocation(mProgram, "uResolution");
    mPtSizeHandler = glGetUniformLocation(mProgram, "uPtSize");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void DotRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void DotRenderer::initBuffer() {
    //empty implementation for this case
}

void DotRenderer::onBeforeInit() {}

void DotRenderer::onPostInit(bool success) {}

void DotRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

