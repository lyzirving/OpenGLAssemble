//
// Created by lyzirving on 2022/6/9.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <Eigen/Dense>
#include <math.h>

#include "CurveRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CurveRenderer"

CurveRenderer::CurveRenderer(const char *name)
        : BaseRendererProgram(name),
          mVaHandler(0),
          mColorHandler(0),
          mResolutionHandler(0),
          mStartHandler(0),
          mControlHandler(0),
          mEndHandler(0),
          mLineWidthHandler(0),
          mVbo() {}

CurveRenderer::~CurveRenderer() = default;

void CurveRenderer::drawCurve(const Point2d &startPt, const Point2d &controlPt,
                              const Point2d &endPt, uint32_t lineWidth) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float step = 0.025;
    uint32_t segCnt = 1.f / step;
    float va[segCnt * 2 * 3];
    for(uint32_t i = 0; i < segCnt; i++) {
        va[i * 6 + 0] = i * step;
        va[i * 6 + 1] = i * step + step;
        va[i * 6 + 2] = 1.f;
        va[i * 6 + 3] = i * step;
        va[i * 6 + 4] = i * step + step;
        va[i * 6 + 5] = -1.f;
    }

    glUseProgram(mProgram);
    glEnableVertexAttribArray(mVaHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, segCnt * 2 * 3 * sizeof(GLfloat), va, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVaHandler, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    glUniform4f(mColorHandler, 1.f, 0.f, 0.f, 1.f);
    glUniform2f(mResolutionHandler, mViewport.mWidth, mViewport.mHeight);
    glUniform2f(mStartHandler, startPt.mX, startPt.mY);
    glUniform2f(mControlHandler, controlPt.mX, controlPt.mY);
    glUniform2f(mEndHandler, endPt.mX, endPt.mY);
    glUniform1f(mLineWidthHandler, lineWidth);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, segCnt * 2);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVaHandler);
    glUseProgram(0);
}

bool CurveRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/curve_vertex_shader.glsl"),
            GlHelper::readAssets("shader/curve_fragment_shader.glsl"));
    return mProgram != 0;
}

void CurveRenderer::initHandler() {
    mVaHandler = glGetAttribLocation(mProgram, "aVertexAttribute");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mResolutionHandler = glGetUniformLocation(mProgram, "uResolution");
    mStartHandler = glGetUniformLocation(mProgram, "uStart");
    mControlHandler = glGetUniformLocation(mProgram, "uControl");
    mEndHandler = glGetUniformLocation(mProgram, "uEnd");
    mLineWidthHandler = glGetUniformLocation(mProgram, "uLineWidth");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void CurveRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void CurveRenderer::initBuffer() {
    //empty implementation for this case
}

void CurveRenderer::onBeforeInit() {}

void CurveRenderer::onPostInit(bool success) {}

void CurveRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

