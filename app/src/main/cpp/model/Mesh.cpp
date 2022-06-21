//
// Created by lyzirving on 2022/6/18.
//
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include "Mesh.h"
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
             mVao(0), mVbo(0), mEbo(0) {}

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices,
           std::vector<Texture> &&textures)
           : mVertices(std::move(vertices)), mIndices(std::move(indices)),
             mTextures(std::move(textures)), mVao(0), mVbo(0), mEbo(0) {}

Mesh::~Mesh() {
    release();
}

void Mesh::release() {
    //todo release Vao, Vbo and Ebo
    {
        std::vector<Vertex> tmp;
        mVertices.swap(tmp);
    }
    {
        std::vector<unsigned int> tmp;
        mIndices.swap(tmp);
    }
    {
        if (!mTextures.empty()) {
            for (auto &tex : mTextures)
                ResourceManager::get()->releaseTexture(tex.path, tex.textureId);
        }
        std::vector<Texture> tmp;
        mTextures.swap(tmp);
    }
}

void Mesh::setupMesh() {
    if(mVao != 0) {
        LogW("mesh has been already initialized");
        return;
    }
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);

    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    // vertex's handler is position 0 in vertex shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // normal's handler is position 1 in vertex shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // texture coordinate's handler is position 2 in vertex shader
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
    LogI("mesh setup");
}

