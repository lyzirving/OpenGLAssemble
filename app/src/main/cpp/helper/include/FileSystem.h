//
// Created by lyzirving on 2022/6/19.
//
#ifndef OPENGLASSEMBLE_FILESYSTEM_H
#define OPENGLASSEMBLE_FILESYSTEM_H

#include <string>

#define ROOT "/sdcard/test"

class FileSystem {
public:

    static std::string getShader(const char *name) {
        std::string path("shader/");
        path.append(name);
        path.append(".glsl");
        return getPath(path);
    }

    static std::string getObj(const char *name) {
        std::string path(name);
        path.append("/");
        path.append(name);
        path.append(".obj");
        return getPath(path);
    }

    static std::string getPath(const char *path) {
        return getPath(std::string(path));
    }

    static std::string getPath(const std::string& path) {
        static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    typedef std::string (*Builder)(const std::string &path);

    static std::string getPathRelativeRoot(const std::string &path) {
        return ROOT + std::string("/") + path;
    }

    static Builder getPathBuilder() {
        return &FileSystem::getPathRelativeRoot;
    }
};

#endif //OPENGLASSEMBLE_FILESYSTEM_H
