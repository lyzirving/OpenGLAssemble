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

struct Light {
    glm::vec3 position;

    /**
     * @brief: Ka, Kd and Ks is the color of ambient, diffuse and specular
     */
    glm::vec3 Ka, Kd, Ks;
    /**
     * @brief: Ka, Kd and Ks is the intensity of ambient, diffuse and specular
     */
    float Ia, Id, Is;
    float shininess;

    Light() : position(0.f),
              Ka(1.f), Kd(1.f), Ks(1.f),
              Ia(0.1f), Id(0.5f), Is(1.f),
              shininess(64.f) {}
};

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

    void computeNormalM(const glm::mat4 &inputModelM, glm::mat3 &out);

private:
    Light mLight;
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<Model> mModel;
    std::shared_ptr<Shader> mShader;

    glm::mat4 mProjectionM;

    /**
     * @brief: Normals got from .obj is computed in local coordinate system.
     *         When we use normals in shader, we need transform them into world coordinate system.
     *         If model is rotated or scaled, we should also apply these transformation to normals.
     *         But if disproportionate scale transform is added to model, the normal will be disturbed,
     *         so we use transpose(inverse(M)) to fix the influence brought by disproportionate scale transform.
     *         Since normal is a direction, translation should not affect normal, so the Normal matrix
     *         should be the left-top 3x3 part of transpose(inverse(M)).
     *
     *  @date: 2022/07/30
     *  @author: lyzirving
     */
    glm::mat3 mNormalM;
};

#endif //OPENGLASSEMBLE_SCENE3D_H
