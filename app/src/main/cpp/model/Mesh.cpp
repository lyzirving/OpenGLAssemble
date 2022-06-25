//
// Created by lyzirving on 2022/6/18.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "Mesh.h"
#include "Shader.h"
#include "GlHelper.h"
#include "Constant.h"
#include "ResourceManager.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

Mesh::Mesh() : mVertices(), mIndices(), mTextures(),
               mVao(0), mVbo(0), mEbo(0) {}

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Texture> &textures)
           : mVertices(vertices), mIndices(indices), mTextures(textures),
             mVao(0), mVbo(0), mEbo(0) {
    setupMesh();
}

Mesh::~Mesh() {
    release();
}

void Mesh::draw(const std::shared_ptr<Shader> &shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    bool match{false};
    int index{0};
    for(auto &texture : mTextures) {
        std::string number;
        std::string name = texture.type;
        if (name == tex::diffuse) {
            number = std::to_string(diffuseNr++);
            match = true;
        } else if (name == tex::specular) {
            number = std::to_string(specularNr++);
            match = true;
        }
        if(match) {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, texture.textureId);
            shader->setInt(name + number, index);
            GlHelper::checkGlError("bind texture", "Mesh");
            LogI("bind texture (%s)", (name + number).c_str());
            index++;
        } else {
            LogE("texture type(%s) does not match", name.c_str());
        }
        match = false;
    }

    GlHelper::checkGlError("before bind vao", "Mesh");
    glBindVertexArray(mVao);
    GlHelper::checkGlError("before draw elements", "Mesh");
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    GlHelper::checkGlError("after draw elements", "Mesh");
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::release() {
    if (!mVertices.empty()) {
        std::vector<Vertex> tmp;
        mVertices.swap(tmp);
    }
    if (!mIndices.empty()) {
        std::vector<unsigned int> tmp;
        mIndices.swap(tmp);
    }
    if (!mTextures.empty()) {
        for (auto &tex : mTextures)
            ResourceManager::get()->releaseTexture(tex.path, tex.textureId);
        std::vector<Texture> tmp;
        mTextures.swap(tmp);
    }
    if (mEbo != 0) {
        glDeleteBuffers(1, &mEbo);
        mEbo = 0;
    }
    if (mVbo != 0) {
        glDeleteBuffers(1, &mVbo);
        mVbo = 0;
    }
    if (mVao != 0) {
        glDeleteVertexArrays(1, &mVao);
        mVao = 0;
    }
}

void Mesh::setupMesh() {
    // **********create buffers**********
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);
    // load vertex data into array buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
    // load indices data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
    // **********finish create buffer **********

    // create vao
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    // bind buffer to vao
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    // set the vertex attribute pointer, vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);
    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
    LogI("mesh setup");
}

