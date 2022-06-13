//
// Created by lyzirving on 2022/6/9.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cmath>

#include "CurveRenderer.h"
#include "DotRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CurveRenderer"

#define DEBUG_POINT 1

CurveRenderer::CurveRenderer(const char *name)
        : BaseRendererProgram(name),
          mVaHandler(0),
          mColorHandler(0),
          mResolutionHandler(0),
          mStartHandler(0),
          mControlHandler(0),
          mEndHandler(0),
          mLineWidthHandler(0),
          mThresholdHandler(0),
          mVbo(),
          mDotRenderer(new DotRenderer(renderer::DOT_RENDERER)),
          mStep(0.025f),
          mVa(nullptr),
          mSegmentCnt(0),
          mThreshold(0.8f) {}

CurveRenderer::~CurveRenderer() {
    delete mDotRenderer;
    if (mVa)
        std::free(mVa);
}

void CurveRenderer::drawCurve(const Point2d &startPt, const Point2d &controlPt, const Point2d &endPt,
        uint32_t lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }

    glUseProgram(mProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableVertexAttribArray(mVaHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, mSegmentCnt * 2 * 3 * sizeof(GLfloat), mVa, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVaHandler, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    glUniform4f(mColorHandler, CHANNEL_R(color), CHANNEL_G(color), CHANNEL_B(color), CHANNEL_A(color));
    glUniform2f(mResolutionHandler, mViewport.mWidth, mViewport.mHeight);
    glUniform2f(mStartHandler, startPt.mX, startPt.mY);
    glUniform2f(mControlHandler, controlPt.mX, controlPt.mY);
    glUniform2f(mEndHandler, endPt.mX, endPt.mY);
    glUniform1f(mLineWidthHandler, lineWidth);
    glUniform1f(mThresholdHandler, mThreshold);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, mSegmentCnt * 2);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVaHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);

#if DEBUG_POINT
    Point2d dots[3];
    dots[0].mX = startPt.mX;
    dots[0].mY = startPt.mY;
    dots[1].mX = endPt.mX;
    dots[1].mY = endPt.mY;
    dots[2].mX = controlPt.mX;
    dots[2].mY = controlPt.mY;
    mDotRenderer->updateViewport(mViewport.mStartX, mViewport.mStartY, mViewport.mWidth, mViewport.mHeight);
    mDotRenderer->drawDot(dots, 3, 10);
#endif
}

void CurveRenderer::drawLine(const Point2d &startPt, const Point2d &endPt, uint32_t lineWidth, uint32_t color) {
    Point2d control((startPt.mX + endPt.mX) * 0.5, (startPt.mY + endPt.mY) * 0.5);
    drawCurve(startPt, control, endPt, lineWidth, color);
}

bool CurveRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/curve_vertex_shader.glsl"),
            GlHelper::readAssets("shader/curve_fragment_shader.glsl"));
    if(mProgram == 0)
        goto fail;
#if DEBUG_POINT
    if(!mDotRenderer->init())
        goto fail;
#endif

    return true;
    fail:
    return false;
}

void CurveRenderer::initHandler() {
    mVaHandler = glGetAttribLocation(mProgram, "aVertexAttribute");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mResolutionHandler = glGetUniformLocation(mProgram, "uResolution");
    mStartHandler = glGetUniformLocation(mProgram, "uStart");
    mControlHandler = glGetUniformLocation(mProgram, "uControl");
    mEndHandler = glGetUniformLocation(mProgram, "uEnd");
    mLineWidthHandler = glGetUniformLocation(mProgram, "uLineWidth");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void CurveRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void CurveRenderer::initBuffer() {
    uint32_t segCnt = 1.f / mStep;
    mSegmentCnt = segCnt + 1;
    mVa = static_cast<float *>(std::calloc(mSegmentCnt * 2 * 3, sizeof(float)));
    for(uint32_t i = 0; i < segCnt; i++) {
        mVa[i * 6 + 0] = i * mStep;
        mVa[i * 6 + 1] = i * mStep + mStep;
        mVa[i * 6 + 2] = 1.f;
        mVa[i * 6 + 3] = i * mStep;
        mVa[i * 6 + 4] = i * mStep + mStep;
        mVa[i * 6 + 5] = -1.f;
    }
    // set for the last segment
    mVa[segCnt * 6 + 0] = 1.f;
    mVa[segCnt * 6 + 1] = 1.f + mStep;
    mVa[segCnt * 6 + 2] = 1.f;
    mVa[segCnt * 6 + 3] = 1.f;
    mVa[segCnt * 6 + 4] = 1.f + mStep;
    mVa[segCnt * 6 + 5] = -1.f;
}

void CurveRenderer::onBeforeInit() {}

void CurveRenderer::onPostInit(bool success) {}

void CurveRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
#if DEBUG_POINT
    if(mDotRenderer)
        mDotRenderer->release();
#endif
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

