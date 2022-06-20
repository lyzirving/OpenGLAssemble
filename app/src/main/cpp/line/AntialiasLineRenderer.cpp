//
// Created by lyzirving on 2022/5/24.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cmath>

#include "AntialiasLineRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "VectorHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AntialiasLineRenderer"

AntialiasLineRenderer::AntialiasLineRenderer(const char *name)
        : BaseRendererProgram(name),
          mVertexHandler(0),
          mTexCoordHandler(0),
          mColorHandler(0),
          mThresholdHandler(0),
          mThreshold(0.4f),
          mColor(),
          mVbo() {}

AntialiasLineRenderer::~AntialiasLineRenderer() = default;

void AntialiasLineRenderer::drawSegment(const Point2d &startPt, const Point2d &endPt, uint32_t lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    Polygon2d polygon{};
    VectorHelper::segmentToPolygon(&polygon, startPt, endPt, lineWidth);

    float polygonVertex[4 * 4];
    float width = polygon.width();
    float height = polygon.height();

    /**
     * We use a vec4's last two component(z and w) to store this rectangle(combined by two triangles)'s width and height.
     * The width and height will be sent to fragment shader via a flat variable, which will not be interpolated through the transmission.
     * The situation that every vertex stores width and height means it cost some extra memory to store useless variable, since
     * the width and height should be stored for one time.But there is no other effective solution right now.
     * todo: how to transfer the width and height just for one time?
     */
    VectorHelper::vertex2d(polygonVertex, polygon.mLeftTop, mViewport);
    polygonVertex[2] = width;
    polygonVertex[3] = height;

    VectorHelper::vertex2d(polygonVertex + 4, polygon.mLeftBottom, mViewport);
    polygonVertex[6] = width;
    polygonVertex[7] = height;

    VectorHelper::vertex2d(polygonVertex + 8, polygon.mRightTop, mViewport);
    polygonVertex[10] = width;
    polygonVertex[11] = height;

    VectorHelper::vertex2d(polygonVertex + 12, polygon.mRightBottom, mViewport);
    polygonVertex[14] = width;
    polygonVertex[15] = height;

    glUseProgram(mProgram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), polygonVertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void AntialiasLineRenderer::drawLines(Point2d *points, uint32_t count, uint32_t lineWidth, uint32_t color) {
    if(mViewport.mWidth <= 0 || mViewport.mHeight <= 0) {
        LogI("(%s) invalid view port", mName.c_str());
        return;
    }
    float polygonVertexArray[(count - 1) * 6 * 4];
    for(uint32_t i = 1; i < count; i++) {
        Polygon2d polygon{};
        VectorHelper::segmentToPolygon(&polygon, points[i - 1], points[i], lineWidth);
        float width = polygon.width();
        float height = polygon.height();

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4, polygon.mLeftTop, mViewport);
        /**
         * We use a vec4's last two component(z and w) to store this rectangle(combined by two triangles)'s width and height.
         * The width and height will be sent to fragment shader via a flat variable, which will not be interpolated through the transmission.
         * The situation that every vertex stores width and height means it cost some extra memory to store useless variable, since
         * the width and height should be stored for one time.But there is no other effective solution right now.
         * todo: how to transfer the width and height just for one time?
         */
        polygonVertexArray[(i - 1) * 6 * 4 + 2] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 3] = height;

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4 + 4, polygon.mLeftBottom, mViewport);
        polygonVertexArray[(i - 1) * 6 * 4 + 6] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 7] = height;

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4 + 8, polygon.mRightTop, mViewport);
        polygonVertexArray[(i - 1) * 6 * 4 + 10] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 11] = height;

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4 + 12, polygon.mRightTop, mViewport);
        polygonVertexArray[(i - 1) * 6 * 4 + 14] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 15] = height;

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4 + 16, polygon.mLeftBottom, mViewport);
        polygonVertexArray[(i - 1) * 6 * 4 + 18] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 19] = height;

        VectorHelper::vertex2d(polygonVertexArray + (i - 1) * 6 * 4 + 20, polygon.mRightBottom, mViewport);
        polygonVertexArray[(i - 1) * 6 * 4 + 22] = width;
        polygonVertexArray[(i - 1) * 6 * 4 + 23] = height;
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
    glBufferData(GL_ARRAY_BUFFER, (count - 1) * 6 * 4 * sizeof(GLfloat),
                 polygonVertexArray, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
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

    glDrawArrays(GL_TRIANGLES, 0, (count - 1) * 6);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

bool AntialiasLineRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(
            GlHelper::readAssets("shader/anti_alias_line_vertex_shader.glsl"),
            GlHelper::readAssets("shader/anti_alias_line_fragment_shader.glsl"));
    return mProgram != 0;
}

void AntialiasLineRenderer::initHandler() {
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mTexCoordHandler = glGetAttribLocation(mProgram, "aTexCoords");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
}

void AntialiasLineRenderer::initCoordinate() {
    glGenBuffers(2, mVbo);
}

void AntialiasLineRenderer::initBuffer() {
    //empty implementation for this case
}

void AntialiasLineRenderer::onBeforeInit() {}

void AntialiasLineRenderer::onPostInit(bool success) {}

void AntialiasLineRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(2, mVbo);
    BaseRendererProgram::release();
}

