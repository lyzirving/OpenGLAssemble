//
// Created by lyzirving on 2022/6/20.
//
#ifndef OPENGLASSEMBLE_SCENE3D_H
#define OPENGLASSEMBLE_SCENE3D_H

#include <glm/glm.hpp>
#include "RendererContext.h"

class Camera;
class Model;
class Shader;

class Scene3d : public RendererContext {
public:
    Scene3d(const char *name);
    ~Scene3d();

    virtual void draw() override;

    void adjustFov(float ratio);
    void liftUpVision(float ratio);
    void rotateModel(int angle);

protected:
    virtual bool onPrepare() override;
    virtual void onQuit() override;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<Model> mModel;
    std::shared_ptr<Shader> mShader;

    glm::mat4x4 mProjectionM;

    glm::vec3 mLightColor;
    float mAmbientCoefficient;
};

#endif //OPENGLASSEMBLE_SCENE3D_H
