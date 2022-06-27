//
// Created by lyzirving on 2022/6/18.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Shader.h"
#include "Constant.h"
#include "ResourceManager.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Model"

Model::Model(const char *path) : mMeshes(), mDirectory(), mModelM(),
                                 mMaxPos(), mMinPos() {
    loadModel(path);
}

Model::~Model() {
    release();
}

void Model::draw(const std::shared_ptr<Shader> &shader) {
    mModelM = glm::mat4(1.f);
    /**
     * In code, we do translate first and then scale.
     * But actually, math performs scale first, and then rotate.
     * Because glm performs matrix multiplication, we must follow the order mentioned bellow.
     */
    // translate it to the center of the scene
    mModelM = glm::translate(mModelM, glm::vec3(0.f, -0.5f, 0.f));
    // it's a bit too big for our scene, so scale it down
    mModelM = glm::scale(mModelM, glm::vec3(0.1f, 0.1f, 0.1f));
    shader->setMat4(shader::model, mModelM);
    for (auto &mesh : mMeshes)
        mesh.draw(shader);
}

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
    LogI("load scene from(%s), begin to parse", path.c_str());
    LogI("********************************");
    processNode(scene->mRootNode, scene);
    LogI("********************************");
    LogI("finish parse node");
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
        aiString texName;
        material->GetTexture(type, i, &texName);
        std::string texPath;
        texPath = mDirectory + '/' + texName.C_Str();
        Texture texUnit;
        texUnit.textureId = ResourceManager::get()->loadTextureFromFile(texPath);
        texUnit.type = typeName;
        texUnit.path = texPath;
        textures.push_back(texUnit);
    }
    return textures;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // note that Node only contains index of object
    // Scene contains all the meshes and elements which are needed for rendering
    // so we need to get objects from Scene by their index
    LogI("node's mesh num(%u), node's children count (%u)",
            node->mNumMeshes, node->mNumChildren);

    // process mesh for current node if the node has any meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        if(mesh != nullptr)
            mMeshes.push_back(std::move(processMesh(mesh, scene)));
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
    LogI("mesh' vertices num(%u), mesh's face num(%u), mesh has material (%s)",
         mesh->mNumVertices, mesh->mNumFaces,
         (mesh->mMaterialIndex >= 0) ? "true" : "false");

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        updateMaxMinPosition(vertex.position);

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if(mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
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
        if(material != nullptr) {
            std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                tex::diffuse);
            textures.insert(textures.end(), diffuse.begin(), diffuse.end());

            std::vector<Texture> specular = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                 tex::specular);
            textures.insert(textures.end(), specular.begin(), specular.end());
        }
    }

    Mesh result{};
    result.mVertices = vertices;
    result.mIndices = indices;
    result.mTextures = textures;
    result.setupMesh();

    return result;
}

void Model::release() {
    LogFunctionEnter;
    if (!mMeshes.empty()) {
        for (auto &mesh : mMeshes)
            mesh.release();
        std::vector<Mesh> tmp;
        mMeshes.swap(tmp);
    }
}

void Model::updateMaxMinPosition(const glm::vec3 &vertex) {
    mMaxPos.x = vertex.x >= mMaxPos.x ? vertex.x : mMaxPos.x;
    mMaxPos.y = vertex.y >= mMaxPos.y ? vertex.y : mMaxPos.y;
    mMaxPos.z = vertex.z >= mMaxPos.z ? vertex.z : mMaxPos.z;

    mMinPos.x = vertex.x < mMinPos.x ? vertex.x : mMinPos.x;
    mMinPos.y = vertex.y < mMinPos.y ? vertex.y : mMinPos.y;
    mMinPos.z = vertex.z < mMinPos.z ? vertex.z : mMinPos.z;
}
