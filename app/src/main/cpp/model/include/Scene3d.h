//
// Created by lyzirving on 2022/6/20.
//
#ifndef OPENGLASSEMBLE_SCENE3D_H
#define OPENGLASSEMBLE_SCENE3D_H

#include "RendererContext.h"

class Model;

class Scene3d : public RendererContext {
public:
    Scene3d(const char *name);
    ~Scene3d();

    virtual void draw() override;

protected:
    virtual bool onPrepare() override;
    virtual void onQuit() override;

private:
    std::shared_ptr<Model> mModel;
};

#endif //OPENGLASSEMBLE_SCENE3D_H
