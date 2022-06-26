//
// Created by lyzirving on 2022/6/21.
//
#ifndef OPENGLASSEMBLE_SHADER_H
#define OPENGLASSEMBLE_SHADER_H

#include <glm/glm.hpp>

#include <string>

class Shader {
public:
    Shader(const char *vertexShader, const char *fragmentShader);
    ~Shader();

    void use(bool active);
    bool valid();

    void setInt(const std::string &name, int value) const;
    void setMat4(const std::string &name, const glm::mat4x4 &mat) const;

    unsigned int mProgram;
};

#endif //OPENGLASSEMBLE_SHADER_H
