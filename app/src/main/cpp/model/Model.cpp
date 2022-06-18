//
// Created by lyzirving on 2022/6/18.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "ResourceManager.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Model"

Model::Model(const char *path) : mMeshes(), mDirectory() {
    loadModel(path);
}

Model::~Model() {}

bool Model::loadModel(const std::string &path) {
    // use assimp load model and get aiScene
    Assimp::Importer importer;
    // aiProcess_Triangulate forces all primitive into triangles
    // aiProcess_FlipUVs will flip the y axis of texture coordinate
    // find more post process cmd here: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
    const aiScene *scene =  importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LogE("fail to load model from (%s), reason = %s", path.c_str(), importer.GetErrorString());
        goto fail;
    }
    mDirectory = path.substr(0, path.find_last_of('/'));
    LogI("succeed to load src from(%s)", path.c_str());
    processNode(scene->mRootNode, scene);

    return true;

    fail:
    return false;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material,
                                                 aiTextureType type,
                                                 const std::string typeName) {
    std::vector<Texture> textures;
    unsigned int count = material->GetTextureCount(type);
    for(unsigned int i = 0; i < count; i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        Texture unit;
        unit.textureId = ResourceManager::loadTextureFromFile(str.C_Str(), mDirectory);
        unit.type = typeName;
        unit.path = str.C_Str();
        textures.push_back(unit);
    }
    return textures;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // note that Node only contains index of object
    // Scene contains all the meshes and elements which are needed for rendering
    // so we need to get objects from Scene by their index

    // process mesh for current node if the node has any meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(processMesh(mesh, scene));
    }

    // process the children for current node
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if(mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }
    }

    // process the indices
    // every face represent a primitive
    // we use flag aiProcess_Triangulate to import the model, so a single primitive is a triangle
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // every mesh only has one Material
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                            "texture_diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());

        std::vector<Texture> specular = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                             "texture_specular");
        textures.insert(textures.end(), specular.begin(), specular.end());
    }

    return Mesh(vertices, indices, textures);
}
