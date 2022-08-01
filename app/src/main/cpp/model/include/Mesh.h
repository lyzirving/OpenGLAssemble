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
    Vertex(Vertex &&other) : position(std::move(other.position)), normal(std::move(other.normal)),
                             texCoords(std::move(other.texCoords)) {}

    Vertex &operator=(const Vertex &other) {
        if (this != &other) {
            this->position = other.position;
            this->normal = other.normal;
            this->texCoords = other.texCoords;
        }
        return *this;
    }
    Vertex& operator =(Vertex &&other) {
        if(this != &other) {
            this->position = std::move(other.position);
            this->normal = std::move(other.normal);
            this->texCoords = std::move(other.texCoords);
        }
        return *this;
    }
};

struct Texture {
    unsigned int textureId;
    std::string path;
    std::string type;

    Texture() : textureId(0), path(), type() {}
    Texture(const Texture &other) : textureId(other.textureId), path(other.path),
                                    type(other.type) {}
    Texture(Texture &&other) : textureId(other.textureId), path(std::move(other.path)),
                               type(std::move(other.type)) {
        other.textureId = 0;
    }
    Texture& operator =(const Texture &other) {
        if(this != &other) {
            this->textureId = other.textureId;
            this->path = other.path;
            this->type = other.type;
        }
        return *this;
    }
    Texture& operator =(Texture &&other) {
        if(this != &other) {
            this->textureId = other.textureId;
            this->path = std::move(other.path);
            this->type = std::move(other.type);
            other.textureId = 0;
        }
        return *this;
    }
};

class Mesh {
public:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture> mTextures;
    glm::vec3 mMaterialKa;
    glm::vec3 mMaterialKd;
    glm::vec3 mMaterialKs;

    Mesh();
    Mesh(Mesh &&other) noexcept;
    Mesh& operator =(Mesh &&other) noexcept;
    ~Mesh();

    void draw(const std::shared_ptr<Shader> &shader);
    void release();
    void setupMesh();
private:
    unsigned int mVao, mVbo, mEbo;
};

#endif //OPENGLASSEMBLE_MESH_H
