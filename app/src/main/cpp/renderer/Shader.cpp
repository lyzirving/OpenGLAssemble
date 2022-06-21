//
// Created by lyzirving on 2022/6/21.
//
#include <GLES2/gl2.h>

#include "Shader.h"
#include "GlHelper.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

Shader::Shader(const char *vertexShader, const char *fragmentShader) {
    mProgram = GlHelper::buildProgram(GlHelper::readAssets(vertexShader),
                                      GlHelper::readAssets(fragmentShader));
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

