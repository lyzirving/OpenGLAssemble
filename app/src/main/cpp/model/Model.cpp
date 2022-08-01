//
// Created by lyzirving on 2022/6/18.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

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

Model::Model(const char *path) : mMeshes(), mDirectory(),
                                 mScaleFitW(1.f),
                                 mFitWindowM(1.f),
                                 mModelM(1.f),
                                 mTransM(1.f),
                                 mRotateM(1.f),
                                 mMaxPos(0.f), mMinPos(0.f),
                                 mMaxPosFitW(0.f), mMinPosFitW(0.f),
                                 mChange(false) {
    loadModel(path);
}

Model::~Model() {
    release();
}

void Model::computeCentralM() {
    float centerX = (mMaxPos.x + mMinPos.x) * 0.5f;
    float centerY = (mMaxPos.y + mMinPos.y) * 0.5f;
    float centerZ = (mMaxPos.z + mMinPos.z) * 0.5f;

    float intervalX =(mMaxPos.x - mMinPos.x);
    float intervalY =(mMaxPos.y - mMinPos.y);
    float intervalZ =(mMaxPos.z - mMinPos.z);
    mScaleFitW = 2.f / std::max(std::max(intervalX, intervalY), intervalZ);

    mMaxPosFitW.x = (mMaxPos.x - centerX) * mScaleFitW;
    mMaxPosFitW.y = (mMaxPos.y - centerY) * mScaleFitW;
    mMaxPosFitW.z = (mMaxPos.z - centerZ) * mScaleFitW;

    mMinPosFitW.x = (mMinPos.x - centerX) * mScaleFitW;
    mMinPosFitW.y = (mMinPos.y - centerY) * mScaleFitW;
    mMinPosFitW.z = (mMinPos.z - centerZ) * mScaleFitW;

    LogI("\nmax(%f, %f, %f), fit window max(%f, %f, %f)\n"
         "min(%f, %f, %f), fit window min(%f, %f, %f)\n"
         "center(%f, %f, %f)\n"
         "interval(%f, %f, %f)\n"
         "scale(%f)",
         mMaxPos.x, mMaxPos.y, mMaxPos.z, mMaxPosFitW.x, mMaxPosFitW.y, mMaxPosFitW.z,
         mMinPos.x, mMinPos.y, mMinPos.z, mMinPosFitW.x, mMinPosFitW.y, mMinPosFitW.z,
         centerX, centerY, centerZ, intervalX, intervalY, intervalZ,
         mScaleFitW);

    // center matrix and scale matrix will set the model to the center of viewport
    glm::mat4 centralM = glm::mat4(1.f);
    glm::mat4 scaleM(1.f);

    centralM = glm::translate(centralM, glm::vec3(-centerX, -centerY, -centerZ));
    scaleM = glm::scale(scaleM, glm::vec3(mScaleFitW, mScaleFitW, mScaleFitW));

    mFitWindowM = scaleM * centralM;
    mChange.store(true);
}

void Model::draw(const std::shared_ptr<Shader> &shader) {
    for (auto &mesh : mMeshes)
        mesh.draw(shader);
}

const glm::mat4& Model::getModelMatrix() {
    if (mChange.load())
    {
        mChange.store(false);
        mModelM = glm::mat4(1.f);
        // the matrix will take effect from the right side
        mModelM =  mTransM * mRotateM * mFitWindowM;
    }
    return mModelM;
}

const glm::vec3 & Model::getMaxPos() {
    return mMaxPosFitW;
}

const glm::vec3 & Model::getMinPos() {
    return mMinPosFitW;
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
    computeCentralM();
    LogI("********************************");
    LogI("finish parse node");
    return true;

    fail:
    return false;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material,
                                                 aiTextureType type,
                                                 const std::string &typeName) {
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
    LogI("mesh' vertices num(%u), mesh's face num(%u), mesh has material (%s)",
         mesh->mNumVertices, mesh->mNumFaces,
         (mesh->mMaterialIndex >= 0) ? "true" : "false");

    Mesh result{};

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

        result.mVertices.push_back(vertex);
    }

    // process the indices
    // every face represent a primitive
    // we use flag aiProcess_Triangulate to import the model, so a single primitive is a triangle
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
            result.mIndices.push_back(face.mIndices[j]);
    }

    // every mesh only has one Material
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        if(material != nullptr) {
            std::vector<Texture> diffuse = loadMaterialTextures(material,
                                                                aiTextureType_DIFFUSE,
                                                                tex::diffuse);
            result.mTextures.insert(result.mTextures.end(), diffuse.begin(), diffuse.end());

            std::vector<Texture> specular = loadMaterialTextures(material,
                                                                 aiTextureType_SPECULAR,
                                                                 tex::specular);
            result.mTextures.insert(result.mTextures.end(), specular.begin(), specular.end());

            aiReturn ret{aiReturn::aiReturn_SUCCESS};
            aiColor3D color;
            ret = material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            if(ret == aiReturn::aiReturn_SUCCESS)
                result.mMaterialKd = glm::vec3(color.r, color.g, color.b);

            ret = material->Get(AI_MATKEY_COLOR_SPECULAR, color);
            if(ret == aiReturn::aiReturn_SUCCESS)
                result.mMaterialKs = glm::vec3(color.r, color.g, color.b);

            ret = material->Get(AI_MATKEY_COLOR_AMBIENT, color);
            if(ret == aiReturn::aiReturn_SUCCESS)
                result.mMaterialKa = glm::vec3(color.r, color.g, color.b);
        }
    }
    LogI("loaded texture count[%ld]", static_cast<unsigned long>(result.mTextures.size()));
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

void Model::rotate(int angle, float x, float y, float z) {
    glm::mat4 unit(1.f);
    mRotateM = glm::rotate(unit, glm::radians(float(angle)), glm::vec3(x, y, z));
    mChange.store(true);
}

void Model::updateMaxMinPosition(const glm::vec3 &vertex) {
    mMaxPos.x = vertex.x >= mMaxPos.x ? vertex.x : mMaxPos.x;
    mMaxPos.y = vertex.y >= mMaxPos.y ? vertex.y : mMaxPos.y;
    mMaxPos.z = vertex.z >= mMaxPos.z ? vertex.z : mMaxPos.z;

    mMinPos.x = vertex.x < mMinPos.x ? vertex.x : mMinPos.x;
    mMinPos.y = vertex.y < mMinPos.y ? vertex.y : mMinPos.y;
    mMinPos.z = vertex.z < mMinPos.z ? vertex.z : mMinPos.z;
}
