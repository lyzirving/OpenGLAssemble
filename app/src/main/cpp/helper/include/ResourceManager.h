//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_RESOURCEMANAGER_H
#define OPENGLASSEMBLE_RESOURCEMANAGER_H

#include <string>
#include <unordered_map>

class ResourceManager {
public:
    static ResourceManager *get();

    unsigned int loadTextureFromFile(const std::string &texPath, bool gamma = false);
    void releaseTexture(const std::string &texPath, const unsigned int textureId);

private:
    ResourceManager();
    ~ResourceManager();

    std::unordered_map<std::string, unsigned int> mTexMap;
};

#endif //OPENGLASSEMBLE_RESOURCEMANAGER_H
