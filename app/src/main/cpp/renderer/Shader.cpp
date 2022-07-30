//
// Created by lyzirving on 2022/6/21.
//
#include <glm/gtc/type_ptr.hpp>
#include <GLES2/gl2.h>

#include "Shader.h"
#include "GlHelper.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

#define ASSETS_ROOT "shader"

Shader::Shader(const char *vertexShader, const char *fragmentShader) {
    std::string vShader(ASSETS_ROOT);
    vShader.append("/");
    vShader.append(vertexShader);
    vShader.append(".glsl");

    std::string fShader(ASSETS_ROOT);
    fShader.append("/");
    fShader.append(fragmentShader);
    fShader.append(".glsl");

    mProgram = GlHelper::buildProgram(GlHelper::readAssets(vShader.c_str()),
                                      GlHelper::readAssets(fShader.c_str()));
}

Shader::~Shader() {
    if (valid()) {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

void Shader::use(bool active) {
    if(valid())
        glUseProgram(active ? mProgram : 0);
}

bool Shader::valid() { return mProgram > 0; }

void Shader::setFloat(const std::string &name, float value) const {
    GLint ind = glGetUniformLocation(mProgram, name.c_str());
    glUniform1f(ind, value);
}

void Shader::setInt(const std::string &name, int value) const {
    GLint ind = glGetUniformLocation(mProgram, name.c_str());
    glUniform1i(ind, value);
}

void Shader::setMat4(const std::string &name, const glm::mat4x4 &mat) const {
    GLint ind = glGetUniformLocation(mProgram, name.c_str());
    glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec3) const {
    GLint ind = glGetUniformLocation(mProgram, name.c_str());
    glUniform3f(ind, vec3.x, vec3.y, vec3.z);
}

