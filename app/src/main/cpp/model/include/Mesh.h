//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_MESH_H
#define OPENGLASSEMBLE_MESH_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    // field is public as default in struct
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex() : position(), normal(), texCoords() {}
    Vertex(const Vertex &other) : position(other.position), normal(other.normal),
                             texCoords(other.texCoords) {}
    Vertex(Vertex &&other) : position(other.position), normal(other.normal),
                             texCoords(other.texCoords) {}
};

struct Texture {
    unsigned int textureId;
    std::string path;
    std::string type;

    Texture() : textureId(0), path(), type() {}
    Texture(const Texture &other) : textureId(other.textureId), path(other.path),
                                    type(other.type) {}
    Texture(Texture &&other) : textureId(other.textureId), path(std::move(other.path)),
                               type(std::move(other.type)) {}
};

class Mesh {
public:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture> mTextures;

    Mesh();
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures);
    Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices,
         std::vector<Texture> &&textures);
    ~Mesh();

    void setupMesh();
private:
    unsigned int mVao, mVbo, mEbo;
};

#endif //OPENGLASSEMBLE_MESH_H
