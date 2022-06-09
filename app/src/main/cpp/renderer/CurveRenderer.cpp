//
// Created by lyzirving on 2022/6/9.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <Eigen/Dense>

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
          mVertexHandler(0),
          mColorHandler(0),
          mPointSizeHandler(0),
          mVbo() {}

CurveRenderer::~CurveRenderer() = default;

void CurveRenderer::drawQuadraticCurve(const Point2d &startPt, const Point2d &midPt, const Point2d &endPt) {
    Eigen::Matrix<float, 3, 3> m;
    m << 0.f, 0.f, 1.f,
         0.25f, 0.25f, 1.f,
         1.f, 1.f, 1.f;
    Eigen::Matrix<float, 3, 1> xData = {startPt.mX, midPt.mX, endPt.mX};
    Eigen::Matrix<float, 3, 1> yData = {startPt.mY, midPt.mY, endPt.mY};
    Eigen::Matrix<float, 3, 1> xCoefficient = m.inverse() * xData;
    Eigen::Matrix<float, 3, 1> yCoefficient = m.inverse() * yData;
}

bool CurveRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/curve_vertex_shader.glsl"),
            GlHelper::readAssets("shader/curve_fragment_shader.glsl"));
    return mProgram != 0;
}

void CurveRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mPointSizeHandler = glGetUniformLocation(mProgram, "uPointSizePixel");
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

