//
// Created by lyzirving on 2022/5/22.
//
#ifndef OPENGLASSEMBLE_GLHELPER_H
#define OPENGLASSEMBLE_GLHELPER_H

class _jobject;
struct _JNIEnv;

class GlHelper {
public:
    static unsigned int buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource);
    static bool checkGlError(const char *msg, const char* tag = "");
    static void logProgramInfo(unsigned int program);
    static void logShaderInfo(unsigned int shader);
    static char *readAssets(const char *path);
    static void release();
    static void setAssetsManager(_JNIEnv *env, _jobject *manager);
private:
    GlHelper();
    ~GlHelper();

    static unsigned int createShader(unsigned int type, const char *source);
};

#endif //OPENGLASSEMBLE_GLHELPER_H
