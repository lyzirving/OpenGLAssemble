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

    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &vec3) const;

    unsigned int mProgram;
};

#endif //OPENGLASSEMBLE_SHADER_H
