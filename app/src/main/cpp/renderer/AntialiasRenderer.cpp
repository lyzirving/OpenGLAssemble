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
          mTexCoordHandler(0),
          mColorHandler(0),
          mThresholdHandler(0),
          mViewportHandler(0),
          mThreshold(0.4f),
          mColor(),
          mTexCoordinate(),
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

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), polygonVertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glVertexAttribPointer(mTexCoordHandler, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT, GL_FLOAT, GL_FALSE, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mTexCoordHandler);

    mColor[0] = (GLfloat)CHANNEL_R(color);
    mColor[1] = (GLfloat)CHANNEL_G(color);
    mColor[2] = (GLfloat)CHANNEL_B(color);
    mColor[3] = (GLfloat)CHANNEL_A(color);
    glUniform4f(mColorHandler, mColor[0], mColor[1], mColor[2], mColor[3]);
    glUniform1f(mThresholdHandler, mThreshold);
    glUniform2f(mViewportHandler, mViewport.mWidth, mViewport.mHeight);
    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GlHelper::checkGlError("draw err", mName.c_str());

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

bool AntialiasRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::ANTI_ALIS_LINE_VERTEX_SHADER, shader::ANTI_ALIS_LINE_FRAGMENT_SHADER);
    return mProgram != 0;
}

void AntialiasRenderer::initHandler() {
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mTexCoordHandler = glGetAttribLocation(mProgram, "aTexCoords");
    mColorHandler = glGetUniformLocation(mProgram, "uColor");
    mThresholdHandler = glGetUniformLocation(mProgram, "uThreshold");
    mViewportHandler = glGetUniformLocation(mProgram, "uViewport");
}

void AntialiasRenderer::initCoordinate() {
    glGenBuffers(2, mVbo);

    std::memcpy(mTexCoordinate, texturecoord::TWO_DIMEN_TEXTURE_COORD,
                sizeof(GLfloat) * texturecoord::TWO_DIMENS_TEXTURE_COORD_COUNT * texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, texturecoord::TWO_DIMENS_TEXTURE_COORD_COUNT * texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat),
                 mTexCoordinate, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mTexCoordHandler, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT, GL_FLOAT, GL_FALSE,
                          texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mTexCoordHandler);
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

