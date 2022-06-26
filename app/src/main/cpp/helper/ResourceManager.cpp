//
// Created by lyzirving on 2022/6/18.
//
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>
#include <mutex>

#include "ResourceManager.h"
#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ResourceManager"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::mutex gInstanceMutex{};
ResourceManager *gResourceMgr{nullptr};

ResourceManager::ResourceManager() : mTexMap() {}

ResourceManager::~ResourceManager() = default;

ResourceManager * ResourceManager::get() {
    if (gResourceMgr == nullptr) {
        std::lock_guard<std::mutex> lock(gInstanceMutex);
        if (gResourceMgr == nullptr)
            gResourceMgr = new ResourceManager;
    }
    return gResourceMgr;
}

unsigned int ResourceManager::loadTextureFromFile(const std::string &texPath, bool gamma) {
    auto iterator =  mTexMap.find(texPath);
    if (iterator != mTexMap.end()) {
        LogI("already contains texture from (%s), use cache", texPath.c_str());
        return iterator->second;
    }

    unsigned int textureId(0);
    int width, height, components;
    unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &components, 0);
    if(data) {
        GLenum format;
        if (components == 1)
            format = GL_RED;
        else if (components == 3)
            format = GL_RGB;
        else if (components == 4)
            format = GL_RGBA;
        else {
            LogE("component(%d) format does not match", components);
            goto done;
        }

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        GlHelper::checkGlError("form texture");

        //cache the texture id
        mTexMap.insert(std::unordered_map<std::string, unsigned int>::value_type(texPath, textureId));
        LogI("load texture, size(%d, %d), component(%d), path(%s)", width, height, components, texPath.c_str());
    } else {
        LogE("fail to load byte data from(%s)", texPath.c_str());
    }

    done:
    stbi_image_free(data);
    return textureId;
}

void ResourceManager::releaseTexture(const std::string &texPath, const unsigned int textureId) {
    auto iterator =  mTexMap.find(texPath);
    if (iterator != mTexMap.end()) {
        if (textureId == iterator->second) {
            LogI("texture id(%u), path(%s)", textureId, texPath.c_str());
            glDeleteTextures(1, &textureId);
        } else {
            LogI("texture cache id(%u), input id(%u), delete both, path(%s)", iterator->second, textureId, texPath.c_str());
            glDeleteTextures(1, &textureId);
            glDeleteTextures(1, &(iterator->second));
        }
        mTexMap.erase(iterator);
    } else {
        LogI("texture id(%u) does not match, delete directly, path (%s)", textureId, texPath.c_str());
        glDeleteTextures(1, &textureId);
    }
}
