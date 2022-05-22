//
// Created by lyzirving on 2022/5/22.
//
#ifndef OPENGLASSEMBLE_GLHELPER_H
#define OPENGLASSEMBLE_GLHELPER_H

class GlHelper {
public:
    static unsigned int buildProgram(const char *vertexShaderSource, const char *fragmentShaderSource);
    static bool checkGlError(const char *msg);
private:
    GlHelper();
    ~GlHelper();

    static unsigned int createShader(unsigned int type, const char *source);
    static void logShaderInfo(unsigned int shader);
};

#endif //OPENGLASSEMBLE_GLHELPER_H
