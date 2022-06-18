//
// Created by lyzirving on 2022/6/18.
//
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include "ResourceManager.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ResourceManager"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager() = default;

unsigned int ResourceManager::loadTextureFromFile(const char *path, const std::string &directory,
                                                  bool gamma) {
    std::string fileName(path);
    fileName = directory + '/' + fileName;

    unsigned int textureId(0);
    glGenTextures(1, &textureId);

    int width, height, nComponents;
    unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nComponents, 0);
    if(data) {
        GLenum format;
        if (nComponents == 1)
            format = GL_RED;
        else if (nComponents == 3)
            format = GL_RGB;
        else if (nComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    } else {
        LogE("fail to load texture from(%s)", fileName.c_str());
    }
    stbi_image_free(data);

    return textureId;
}
