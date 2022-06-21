//
// Created by lyzirving on 2022/6/21.
//
#ifndef OPENGLASSEMBLE_SHADER_H
#define OPENGLASSEMBLE_SHADER_H

#include <string>

class Shader {
public:
    Shader(const char *vertexShader, const char *fragmentShader);
    ~Shader();

    void use(bool active);
    bool valid();
protected:
    unsigned int mProgram;
};

#endif //OPENGLASSEMBLE_SHADER_H
