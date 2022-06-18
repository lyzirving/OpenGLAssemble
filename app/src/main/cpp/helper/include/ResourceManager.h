//
// Created by lyzirving on 2022/6/18.
//
#ifndef OPENGLASSEMBLE_RESOURCEMANAGER_H
#define OPENGLASSEMBLE_RESOURCEMANAGER_H

#include <string>

class ResourceManager {
public:
    static unsigned int loadTextureFromFile(const char* path, const std::string &directory,
                                            bool gamma = false);

private:
    ResourceManager();
    ~ResourceManager();
};

#endif //OPENGLASSEMBLE_RESOURCEMANAGER_H
