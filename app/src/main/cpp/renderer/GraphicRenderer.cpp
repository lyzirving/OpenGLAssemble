//
// Created by lyzirving on 2022/5/23.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GraphicRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GraphicRenderer"

#define CHANNEL_R(_COLOR) (((float)(((_COLOR) & 0xFF000000) >> 24) / (float)255))
#define CHANNEL_G(_COLOR) (((float)(((_COLOR) & 0x00FF0000) >> 16) / (float)255))
#define CHANNEL_B(_COLOR) (((float)(((_COLOR) & 0x0000FF00) >> 8) / (float)255))
#define CHANNEL_A(_COLOR) (((float)(((_COLOR) & 0x000000FF)) / (float)255))

GraphicRenderer::GraphicRenderer(const char *name)
        : BaseRendererProgram(name),
          mVertexHandler(0),
          mVertexCntHandler(0),
          mGradientHandler(0),
          mLastVertexCount(0),
          mVbo(),
          mGradientColor() {}

GraphicRenderer::~GraphicRenderer() = default;

void GraphicRenderer::drawLines(float *vertex, uint32_t vertexCnt, uint32_t vertexComponent,
                                uint32_t color, float lineWidth) {
    drawGradientLines(vertex, vertexCnt, vertexComponent, color, color, lineWidth);
}

void GraphicRenderer::drawGradientLines(float *vertex, uint32_t vertexCnt, uint32_t vertexComponent,
                                        uint32_t startColor, uint32_t endColor, float lineWidth) {
    if (!mInitialized) {
        LogE("err, renderer(%s) is not initialized", mName.c_str());
        return;
    }
    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    if (mLastVertexCount != vertexCnt) {
        /**
         * if mLastVertexCount is 0, glBufferData will create a data store for the buffer object
         * if mLastVertexCount is not equal to current count, glBufferData will delete the pre-existing data store, and create a new one
         */
        glBufferData(GL_ARRAY_BUFFER, vertexCnt * vertexComponent * sizeof(GLfloat), vertex, GL_DYNAMIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCnt * vertexComponent * sizeof(GLfloat), vertex);
    }
    mLastVertexCount = vertexCnt;
    glVertexAttribPointer(mVertexHandler, vertexComponent, GL_FLOAT, GL_FALSE, vertexComponent * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);

    mGradientColor[0] = (GLfloat)CHANNEL_R(startColor);
    mGradientColor[1] = (GLfloat)CHANNEL_G(startColor);
    mGradientColor[2] = (GLfloat)CHANNEL_B(startColor);
    mGradientColor[3] = (GLfloat)CHANNEL_A(startColor);

    mGradientColor[4] = (GLfloat)CHANNEL_R(endColor);
    mGradientColor[5] = (GLfloat)CHANNEL_G(endColor);
    mGradientColor[6] = (GLfloat)CHANNEL_B(endColor);
    mGradientColor[7] = (GLfloat)CHANNEL_A(endColor);
    glUniform4fv(mGradientHandler, 2, mGradientColor);

    glUniform1i(mVertexCntHandler, vertexCnt);

    glLineWidth(lineWidth);

    glDrawArrays(GL_LINE_STRIP, 0, vertexCnt);

    glDisableVertexAttribArray(mVertexHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    MatrixUtil::identity(mMatrix);
    glUseProgram(0);
}

bool GraphicRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::GRAPHIC_VERTEX_SHADER, shader::GRAPHIC_FRAGMENT_SHADER);
    return mProgram != 0;
}

void GraphicRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mVertexCntHandler = glGetUniformLocation(mProgram, "uVertexCount");
    mGradientHandler = glGetUniformLocation(mProgram, "uGradient");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void GraphicRenderer::initCoordinate() {
    glGenBuffers(1, mVbo);
}

void GraphicRenderer::initBuffer() {
    //empty implementation for this case
}

void GraphicRenderer::onBeforeInit() {}

void GraphicRenderer::onPostInit(bool success) {}

void GraphicRenderer::release() {
    LogI("renderer(%s) release", mName.c_str());
    glDeleteBuffers(1, mVbo);
    BaseRendererProgram::release();
}

