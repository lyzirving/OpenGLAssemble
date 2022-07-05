//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_MODEL_H
#define OPENGLASSEMBLE_MODEL_H

#include <assimp/material.h>
#include <glm/glm.hpp>

#include "Mesh.h"

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;

class Model {
public:
    Model(const char *path);
    ~Model();

    void draw(const std::shared_ptr<Shader> &shader);
    void release();

private:
    /**
     * load model from specific path
     * @param path: a path that specify the .obj file
     * @return true if success; false otherwise
     */
    bool loadModel(const std::string &path);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material,
                                              aiTextureType type,
                                              const std::string &typeName);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    void updateMaxMinPosition(const glm::vec3 &vertex);

    std::vector<Mesh> mMeshes;
    std::string mDirectory;
    glm::mat4 mModelM;
    glm::vec3 mMaxPos, mMinPos;
};

#endif //OPENGLASSEMBLE_MODEL_H
