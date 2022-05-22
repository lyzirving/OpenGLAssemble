//
// Created by lyzirving on 2022/5/22.
//
#ifndef OPENGLASSEMBLE_BASERENDERERPROGRAM_H
#define OPENGLASSEMBLE_BASERENDERERPROGRAM_H

#include <string>

class BaseRendererProgram {
public:
    BaseRendererProgram(const char *name, const char* vertexShader, const char* fragShader);
    virtual ~BaseRendererProgram();

    bool init();
    virtual void draw(unsigned int textureId) = 0;
    virtual void release();
protected:

    bool initProgram();
    virtual void initHandler() = 0;
    virtual void initCoordinate() = 0;
    virtual void initBuffer() = 0;

    virtual void onBeforeInit() = 0;
    virtual void onPostInit(bool success) = 0;

    bool mInitialized;
    std::string mName;
    unsigned int mProgram;
    float mMatrix[16];
    char *mVertexShader;
    char *mFragmentShader;
};

#endif //OPENGLASSEMBLE_BASERENDERERPROGRAM_H
