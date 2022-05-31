//
// Created by lyzirving on 2022/5/24.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
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
          mTexCoordHandler(0),
          mColorHandler(0),
          mThresholdHandler(0),
          mResolutionHandler(0),
          mLeftAnchorHandler(0),
          mThreshold(0.4f),
          mColor(),
          mVbo() {}

AntialiasRenderer::~AntialiasRenderer() = default;

void AntialiasRenderer::drawSegment(const Point2d &startPt, const Point2d &endPt, uint32_t lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    Polygon2d polygon{};
    VectorHelper::segmentToPolygon(&polygon, startPt, endPt, lineWidth);
    float polygonVertex[8];

    VectorHelper::vertex2d(polygonVertex, polygon.mLeftTop, mViewport);
    VectorHelper::vertex2d(polygonVertex + 2, polygon.mLeftBottom, mViewport);
    VectorHelper::vertex2d(polygonVertex + 4, polygon.mRightTop, mViewport);
    VectorHelper::vertex2d(polygonVertex + 6, polygon.mRightBottom, mViewport);

    glUseProgram(mProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, /*vertex count*/4 * /*component*/2 * sizeof(GLfloat),
                 polygonVertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(mTexCoordHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), texturecoord::TWO_DIMEN_TEXTURE_COORD_STRIP, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mTexCoordHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    mColor[0] = (GLfloat)CHANNEL_R(color);
    mColor[1] = (GLfloat)CHANNEL_G(color);
    mColor[2] = (GLfloat)CHANNEL_B(color);
    mColor[3] = (GLfloat)CHANNEL_A(color);
    glUniform4f(mColorHandler, mColor[0], mColor[1], mColor[2], mColor[3]);
    glUniform1f(mThresholdHandler, mThreshold);
    //notice the viewport we send to fragment shader is the total rectangle's width and height
    glUniform2f(mResolutionHandler, polygon.width(), polygon.height());
    glUniform1f(mLeftAnchorHandler, (polygon.height() * 0.5f ) / polygon.width());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void AntialiasRenderer::drawLines(Point2d *points, uint32_t count, uint32_t lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    //need to pass multiple polygon size
    Polygon2d placeholder{};
    float polygonVertexArray[(count - 1) * 6 * 2];
    for(uint32_t i = 1; i < count; i++) {
        Polygon2d polygon{};
        VectorHelper::segmentToPolygon(&polygon, points[i - 1], points[i], lineWidth);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12, polygon.mLeftTop, mViewport);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12 + 2, polygon.mLeftBottom, mViewport);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12 + 4, polygon.mRightTop, mViewport);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12 + 6, polygon.mRightTop, mViewport);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12 + 8, polygon.mLeftBottom, mViewport);
        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 12 + 10, polygon.mRightBottom, mViewport);
        placeholder = polygon;
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
    glBufferData(GL_ARRAY_BUFFER, (count - 1) * 6 * 2 * sizeof(GLfloat),
                 polygonVertexArray, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(mTexCoordHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, (count - 1) * 6 * 2 * sizeof(GLfloat), textureCoordinate, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mTexCoordHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);
    mColor[0] = (GLfloat)CHANNEL_R(color);
    mColor[1] = (GLfloat)CHANNEL_G(color);
    mColor[2] = (GLfloat)CHANNEL_B(color);
    mColor[3] = (GLfloat)CHANNEL_A(color);
    glUniform4f(mColorHandler, mColor[0], mColor[1], mColor[2], mColor[3]);
    glUniform1f(mThresholdHandler, mThreshold);
    //notice the viewport we send to fragment shader is the total rectangle's width and height
    glUniform2f(mResolutionHandler, placeholder.width(), placeholder.height());
    glUniform1f(mLeftAnchorHandler, (placeholder.height() * 0.5f ) / placeholder.width());

    glDrawArrays(GL_TRIANGLES, 0, (count - 1) * 6);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

bool AntialiasRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/anti_alias_line_vertex_shader.glsl"),
            GlHelper::readAssets("shader/anti_alias_line_fragment_shader.glsl"));
    return mProgram != 0;
}

void AntialiasRenderer::initHandler() {
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mTexCoordHandler = glGetAttribLocation(mProgram, "aTexCoords");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
    mResolutionHandler = glGetUniformLocation(mProgram, "uResolution");
    mLeftAnchorHandler = glGetUniformLocation(mProgram, "uLeftAnchorPos");
}

void AntialiasRenderer::initCoordinate() {
    glGenBuffers(2, mVbo);
}

void AntialiasRenderer::initBuffer() {
    //empty implementation for this case
}

void AntialiasRenderer::onBeforeInit() {}

void AntialiasRenderer::onPostInit(bool success) {}

void AntialiasRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(2, mVbo);
    BaseRendererProgram::release();
}

