//
// Created by lyzirving on 2022/5/22.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdlib>
#include <cstring>
#include <jni.h>
#include <android/asset_manager_jni.h>

#include "GlHelper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GlHelper"

#define NO_TEXTURE 0

static AAssetManager *gManager{nullptr};

GlHelper::GlHelper() = default;

GlHelper::~GlHelper() = default;

unsigned int GlHelper::buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    GLuint vertexShader{0};
    GLuint fragmentShader{0};
    GLuint program{0};
    GLint linkStatus{GL_FALSE};
    if (vertexShaderSource == nullptr || std::strlen(vertexShaderSource) == 0) {
        LogE("input vertex shader is null");
        goto err;
    }
    if (fragmentShaderSource == nullptr || std::strlen(fragmentShaderSource) == 0) {
        LogE("input fragment shader is null");
        goto err;
    }
    vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    if(!vertexShader) {
        LogE("fail to compile vertex shader %s", vertexShaderSource);
        goto err;
    }
    fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if(!fragmentShader) {
        LogE("fail to compile fragment shader %s", fragmentShaderSource);
        goto err;
    }
    program = glCreateProgram();
    if(!program) {
        checkGlError("fail to create program");
        goto err;
    }
    glAttachShader(program, vertexShader);
    if(!checkGlError("fail to attach vertex shader"))
        goto err;

    glAttachShader(program, fragmentShader);
    if(!checkGlError("fail to attach fragment shader"))
        goto err;

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {
        LogE("fail to link program");
        logShaderInfo(program);
        glDeleteProgram(program);
        goto err;
    }
    return program;
    err:
    return 0;
}

bool GlHelper::checkGlError(const char *msg, const char *tag) {
    GLenum ret = glGetError();
    if (ret == GL_NO_ERROR) {
        return true;
    } else {
        LogE("[%s]err: %s, err code(0x%x)", tag, msg, ret);
        return false;
    }
}

unsigned int GlHelper::createShader(unsigned int type, const char *source) {
    //glCreateShader return zero when fail
    GLuint shader = glCreateShader(type);
    if(shader) {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        logShaderInfo(shader);
        if(compileStatus != GL_TRUE) {
            glDeleteShader(shader);
            shader = 0;
        }
    } else {
        LogE("fail to create shader for type(0x%x), err code(0x%x)", type, glGetError());
    }
    return shader;
}

void GlHelper::logShaderInfo(unsigned int shader) {
    if (shader == 0) {
        LogE("invalid input shader");
        return;
    }
    GLint infoLen{0};
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen) {
        char *info = static_cast<char *>(std::malloc(sizeof(char) * infoLen));
        if (info) {
            glGetShaderInfoLog(shader, infoLen, nullptr, info);
            LogI("shader(%u) status: %s", shader, info);
            std::free(info);
        } else {
            LogE("fail to malloc memory for shader(%u) info log", shader);
        }
    }
}

char * GlHelper::readAssets(const char *path) {
    char *res{nullptr};
    AAsset *asset{nullptr};
    off_t len;
    if (gManager == nullptr) {
        LogE("assets manager is null");
        goto done;
    }
    if (path == nullptr || std::strlen(path) == 0) {
        LogE("input path is invalid");
        goto done;
    }
    asset = AAssetManager_open(gManager, path, AASSET_MODE_BUFFER);
    if (asset == nullptr) {
        LogE("fail to open assets, path: %s", path);
        goto done;
    }
    len = AAsset_getLength(asset);
    res = static_cast<char *>(std::malloc(len + 1));
    res[len] = 0;
    AAsset_read(asset, res, len);
    AAsset_close(asset);

    done:
    return res;
}

void GlHelper::setAssetsManager(_JNIEnv *env, _jobject *manager) {
    gManager = AAssetManager_fromJava(env, manager);
}

