//
// Created by liuyuzhou on 2022/5/23.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstring>

#include "TwoDimensRenderer.h"
#include "RendererMetadata.h"
#include "MatrixUtil.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TwoDimensRenderer"

TwoDimensRenderer::TwoDimensRenderer(const char *name)
        : BaseRendererProgram(name),
          mVertexHandler(0), mTexCoordHandler(0), mTwoDimenSampler(0), mMatrixHandler(0),
          mVertex(), mTexCoordinate(), mVbo() {

}

TwoDimensRenderer::~TwoDimensRenderer() = default;

void TwoDimensRenderer::draw(unsigned int textureId) {
    if (!mInitialized) {
        LogE("err, renderer(%s) is not initialized", mName.c_str());
        return;
    }

    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glVertexAttribPointer(mVertexHandler, vertex::TWO_DIMENS_VERTEX_COMPONENT, GL_FLOAT, GL_FALSE, vertex::TWO_DIMENS_VERTEX_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glVertexAttribPointer(mTexCoordHandler, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT, GL_FLOAT, GL_FALSE, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mTexCoordHandler);

    glUniformMatrix4fv(mMatrixHandler, 1, false, mMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(mTwoDimenSampler, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex::TWO_DIMENS_VERTEX_COUNT);

    glDisableVertexAttribArray(mVertexHandler);
    glDisableVertexAttribArray(mTexCoordHandler);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    MatrixUtil::identity(mMatrix);

    glUseProgram(0);
}

void TwoDimensRenderer::flushVertexCoordinate() {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex::TWO_DIMENS_VERTEX_COUNT * vertex::TWO_DIMENS_VERTEX_COMPONENT * sizeof(GLfloat), mVertex);
    glVertexAttribPointer(mVertexHandler, vertex::TWO_DIMENS_VERTEX_COMPONENT, GL_FLOAT, GL_FALSE, vertex::TWO_DIMENS_VERTEX_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);
}

void TwoDimensRenderer::flushTextureCoordinate() {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texturecoord::TWO_DIMENS_TEXTURE_COORD_COUNT * texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), mTexCoordinate);
    glVertexAttribPointer(mTexCoordHandler, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT, GL_FLOAT, GL_FALSE, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mTexCoordHandler);
}

bool TwoDimensRenderer::initProgram() {
    mProgram = GlHelper::buildProgram(shader::TWO_DIMEN_VERTEX_SHADER, shader::TWO_DIMEN_FRAGMENT_SHADER);
    return mProgram != 0;
}

void TwoDimensRenderer::initHandler() {
    mVertexHandler = glGetAttribLocation(mProgram, "aVertexCoords");
    mTexCoordHandler = glGetAttribLocation(mProgram, "aTexCoords");
    mTwoDimenSampler = glGetUniformLocation(mProgram, "uTwoDimensTextureSampler");
    mMatrixHandler = glGetUniformLocation(mProgram, "uMatrix");
}

void TwoDimensRenderer::initCoordinate() {
    std::memcpy(mVertex, vertex::TWO_DIMENS_VERTEX_ARRAY,
                sizeof(GLfloat) * vertex::TWO_DIMENS_VERTEX_COUNT * vertex::TWO_DIMENS_VERTEX_COMPONENT);
    std::memcpy(mTexCoordinate, texturecoord::TWO_DIMEN_TEXTURE_COORD,
                sizeof(GLfloat) * texturecoord::TWO_DIMENS_TEXTURE_COORD_COUNT * texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT);

    glGenBuffers(2, mVbo);

    // bind vertex array buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex::TWO_DIMENS_VERTEX_COUNT * vertex::TWO_DIMENS_VERTEX_COMPONENT * sizeof(GLfloat),
                 mVertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mVertexHandler, vertex::TWO_DIMENS_VERTEX_COMPONENT, GL_FLOAT, GL_FALSE,
            vertex::TWO_DIMENS_VERTEX_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mVertexHandler);

    // bind texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, texturecoord::TWO_DIMENS_TEXTURE_COORD_COUNT * texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat),
                 mTexCoordinate, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(mTexCoordHandler, texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT, GL_FLOAT, GL_FALSE,
            texturecoord::TWO_DIMENS_TEXTURE_COORD_COMPONENT * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(mTexCoordHandler);
}

void TwoDimensRenderer::initBuffer() {
    //empty implementation for this case
}

void TwoDimensRenderer::release() {
    glDeleteBuffers(2, mVbo);
    BaseRendererProgram::release();
}

void TwoDimensRenderer::updateVertexCoordinate(uint32_t viewX, uint32_t viewY,
                                               uint32_t width, uint32_t height) {
    // left-top
    BaseRendererProgram::calculateVertex(mVertex + 0, (float)viewX, (float)viewY);
    // left-bottom
    BaseRendererProgram::calculateVertex(mVertex + 2, (float)viewX, (float)(viewY + height));
    // right-top
    BaseRendererProgram::calculateVertex(mVertex + 4, (float)(viewX + width), (float)viewY);
    // right-bottom
    BaseRendererProgram::calculateVertex(mVertex + 6, (float)(viewX + width), (float)(viewY + height));
    flushVertexCoordinate();
}

void TwoDimensRenderer::updateTextureCoordinate(uint32_t startX, uint32_t startY, uint32_t subWidth,
                                                uint32_t subHeight, uint32_t rawWidth, uint32_t rawHeight,
                                                bool flipX, bool flipY) {
    // left-top
    mTexCoordinate[0] = (float) startX / (float) rawWidth;
    if (flipX)
        mTexCoordinate[0] = float(1) - mTexCoordinate[0];
    mTexCoordinate[1] = (float) startY / (float) rawHeight;
    if (flipY)
        mTexCoordinate[1] = float(1) - mTexCoordinate[1];

    // left-bottom
    mTexCoordinate[2] = (float) startX / (float) rawWidth;
    if (flipX)
        mTexCoordinate[2] = float(1) - mTexCoordinate[2];
    mTexCoordinate[3] = (float) (startY + subHeight) / (float) rawHeight;
    if (flipY)
        mTexCoordinate[3] = float(1) - mTexCoordinate[3];

    // right-top
    mTexCoordinate[4] = (float) (startX + subWidth) / (float) rawWidth;
    if (flipX)
        mTexCoordinate[4] = float(1) - mTexCoordinate[4];
    mTexCoordinate[5] = (float) startY / (float) rawHeight;
    if (flipY)
        mTexCoordinate[5] = float(1) - mTexCoordinate[5];

    // right-bottom
    mTexCoordinate[6] = (float) (startX + subWidth) / (float) rawWidth;
    if (flipX)
        mTexCoordinate[6] = float(1) - mTexCoordinate[6];
    mTexCoordinate[7] = (float) (startY + subHeight) / (float) rawHeight;
    if (flipY)
        mTexCoordinate[7] = float(1) - mTexCoordinate[7];

    flushTextureCoordinate();
}