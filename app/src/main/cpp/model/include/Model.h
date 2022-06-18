//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_MODEL_H
#define OPENGLASSEMBLE_MODEL_H

#include "Mesh.h"

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;
enum aiTextureType;

class Model {
public:
    Model(const char *path);
    ~Model();

private:
    std::vector<Mesh> mMeshes;
    std::string mDirectory;

    bool loadModel(const std::string &path);
    std::vector<Texture> loadMaterialTextures(aiMaterial *material,
                                              aiTextureType type,
                                              const std::string typeName);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

#endif //OPENGLASSEMBLE_MODEL_H
