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
          mVertexHandler(0),
          mTexCoordHandler(0),
          mColorHandler(0),
          mThresholdHandler(0),
          mThreshold(0.4f),
          mVbo() {}

CurveRenderer::~CurveRenderer() = default;

double CurveRenderer::besselQuadratic(double t, bool horizontal, const Point2d &start,
                                      const Point2d &control,
                                      const Point2d &end) {
    if (horizontal) {
        return ((1 - t) * (1 - t) * start.mX + 2 * t * (1 - t) * control.mX + t * t * end.mX);
    } else {
        return ((1 - t) * (1 - t) * start.mY + 2 * t * (1 - t) * control.mY + t * t * end.mY);
    }
}

void CurveRenderer::drawQuadraticCurve(const Point2d &startPt, const Point2d &controlPt,
                                       const Point2d &endPt,
                                       uint32_t lineWidth) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }

    double step = 0.02;
    uint32_t count = std::round(1 / step) + 1;
    float polygonVertexArray[(count - 1) * 6 * 4];
    uint32_t ind = 0;

    for(double t = step; t <= 1; t += step) {
        double lastT = t - step;
        Point2d first{}, second{};
        first.mX = std::round(besselQuadratic(lastT, true, startPt, controlPt, endPt));
        first.mY = std::round(besselQuadratic(lastT, false, startPt, controlPt, endPt));
        second.mX = std::round(besselQuadratic(t, true, startPt, controlPt, endPt));
        second.mY = std::round(besselQuadratic(t, false, startPt, controlPt, endPt));

        Polygon2d polygon{};
        VectorHelper::segmentToPolygon(&polygon, first, second, lineWidth);
        float width = polygon.width();
        float height = polygon.height();

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4, polygon.mLeftTop, mViewport);
        polygonVertexArray[ind * 6 * 4 + 2] = width;
        polygonVertexArray[ind * 6 * 4 + 3] = height;

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4 + 4, polygon.mLeftBottom, mViewport);
        polygonVertexArray[ind * 6 * 4 + 6] = width;
        polygonVertexArray[ind * 6 * 4 + 7] = height;

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4 + 8, polygon.mRightTop, mViewport);
        polygonVertexArray[ind * 6 * 4 + 10] = width;
        polygonVertexArray[ind * 6 * 4 + 11] = height;

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4 + 12, polygon.mRightTop, mViewport);
        polygonVertexArray[ind * 6 * 4 + 14] = width;
        polygonVertexArray[ind * 6 * 4 + 15] = height;

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4 + 16, polygon.mLeftBottom, mViewport);
        polygonVertexArray[ind * 6 * 4 + 18] = width;
        polygonVertexArray[ind * 6 * 4 + 19] = height;

        VectorHelper::vertex2d(polygonVertexArray + ind * 6 * 4 + 20, polygon.mRightBottom, mViewport);
        polygonVertexArray[ind * 6 * 4 + 22] = width;
        polygonVertexArray[ind * 6 * 4 + 23] = height;

        ind++;
    }
    float textureCoordinate[(count - 1) * 6 * 2];
    for (uint32_t i = 0; i < (count - 1); ++i) {
        std::memcpy(textureCoordinate + i * 6 * 2, texturecoord::TWO_DIMEN_TEXTURE_COORD_NORMAL, 6 * 2 * sizeof(float));
    }

    glUseProgram(mProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, (count - 1) * 6 * 4 * sizeof(GLfloat), polygonVertexArray, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(mTexCoordHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, (count - 1) * 6 * 2 * sizeof(GLfloat), textureCoordinate, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mTexCoordHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    glUniform4f(mColorHandler, 1.f, 0.f, 0.f, 1.f);
    glUniform1f(mThresholdHandler, mThreshold);

    glDrawArrays(GL_TRIANGLES, 0, (count - 1) * 6);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

bool CurveRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/continuous_line_vertex_shader.glsl"),
            GlHelper::readAssets("shader/continuous_line_fragment_shader.glsl"));
    return mProgram != 0;
}

void CurveRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mTexCoordHandler = glGetAttribLocation(mProgram, "aTexCoords");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void CurveRenderer::initCoordinate() {
    glGenBuffers(2, mVbo);
}

void CurveRenderer::initBuffer() {
    //empty implementation for this case
}

void CurveRenderer::onBeforeInit() {}

void CurveRenderer::onPostInit(bool success) {}

void CurveRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(2, mVbo);
    BaseRendererProgram::release();
}

