//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_MESH_H
#define OPENGLASSEMBLE_MESH_H

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Shader;

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
    Texture & operator =(const Texture &other) {
        if(this != &other) {
            this->textureId = other.textureId;
            this->path = other.path;
            this->type = other.type;
        }
        return *this;
    }
};

class Mesh {
public:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture> mTextures;

    Mesh();
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures);
    ~Mesh();

    void draw(const std::shared_ptr<Shader> &shader);
    void release();
private:
    void setupMesh();

    unsigned int mVao, mVbo, mEbo;
};

#endif //OPENGLASSEMBLE_MESH_H
