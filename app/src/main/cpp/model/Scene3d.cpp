//
// Created by lyzirving on 2022/6/20.
//
#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene3d.h"
#include "Camera.h"
#include "Shader.h"
#include "Constant.h"
#include "Model.h"
#include "WindowSurface.h"
#include "FileSystem.h"
#include "GlHelper.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene3d"

Scene3d::Scene3d(const char *name) : RendererContext(name),
                                     mCamera(new Camera),
                                     mModel(nullptr),
                                     mShader(nullptr),
                                     mProjectionM(1.f),
                                     mNormalM(1.f),
                                     mLight() {}

Scene3d::~Scene3d() {
    mCamera.reset();
    mModel.reset();
    mShader.reset();
}

void Scene3d::draw() {
    auto it = mWindows.begin();
    while(it != mWindows.end()) {
        std::shared_ptr<WindowSurface> window = it->second;
        window->makeCurrent();

        glClearColor(float(0), float(0), float(0), float(1));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        uint32_t width = window->getWidth();
        uint32_t height = window->getHeight();
        glViewport(0, 0, width, height);

        mShader->use(true);

        const glm::mat4& modelM = mModel->getModelMatrix();
        mShader->setMat4(shader::model, modelM);

        computeNormalM(modelM, mNormalM);
        mShader->setMat3(shader::normalM, mNormalM);

        // View matrix specifies camera's position and sight direction in world coordinate,
        // When all vertices apply view matrix, they are transformed into camera coordinate.
        // In camera coordinate, camera is set at (0, 0, 0), and sight direction is parallel to (0, 0, -1).
        //todo: find the algorithm to compute one camera's z-position that will show the whole model on screen
        //const glm::vec3& modelMaxPos = mModel->getMaxPos();
        //mCamera->moveCameraTo(0, 0, modelMaxPos.z * 15);
        mShader->setMat4(shader::view, mCamera->getViewMatrix());

        // Cond 1: model will be set to center, and vertices will be scaled to [-1, 1] later.
        // Cond 2: camera's position is (0, 0, 3f) as default in world coordinate.
        // Cond 3: the distance from near clip plane to camera is 0.1.
        // According to the three conditions above, in camera coordinate, camera is set at (0, 0, 0).
        // Near clip plane is set at (0, 0, -0.1), and far clip plane is set at (0, 0, -10).
        // Model's center is at (0, 0, -3).
        mProjectionM = glm::perspective(glm::radians(mCamera->getFov()),
                                        float(width) / float(height),
                                        0.1f, 10.f);
        // h = tan[(field of view in y) * 0.5] * near, bottom = -h
        // left = -aspect ratio * h, right = -left
        mShader->setMat4(shader::projection, mProjectionM);

        const glm::vec3 &maxModelPos = mModel->getMaxPos();
        mLight.position.x = std::abs(maxModelPos.x) * (-2.f);
        mLight.position.y = std::abs(maxModelPos.y) * 2.f;
        mShader->setVec3(shader::lightPos, mLight.position);

        mShader->setVec3(shader::lightAmbientRgb, mLight.ambientRgb);
        mShader->setVec3(shader::lightDiffuseRgb, mLight.diffuseRgb);
        mShader->setVec3(shader::lightSpecularRgb, mLight.specularRgb);

        mShader->setFloat(shader::lightKa, mLight.Ka);
        mShader->setFloat(shader::lightKd, mLight.Kd);
        mShader->setFloat(shader::lightKs, mLight.Ks);

        mShader->setFloat(shader::lightShininess, mLight.shininess);

        mShader->setVec3(shader::cameraPos, mCamera->getCameraPosition());

        mModel->draw(mShader);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        mShader->use(false);

        window->swapBuffer();
        it++;
    }
}

bool Scene3d::onPrepare() {
    mShader = std::make_shared<Shader>("model_vs", "model_fs");
    if(!mShader->valid())
        goto fail;

    mModel = std::make_shared<Model>(FileSystem::getObj("spiderman").c_str());
    return true;

    fail:
    return false;
}

void Scene3d::onQuit() {
    mModel.reset();
    mShader.reset();
}

void Scene3d::adjustFov(float ratio) {
    if (mCamera)
    {
        mCamera->adjustFov(ratio);
        sendMessage(MessageId::MESSAGE_REQUEST_DRAW);
    }
}

void Scene3d::computeNormalM(const glm::mat4 &inputModelM, glm::mat3 &out) {
    glm::mat4 tmpMat4(inputModelM);
    tmpMat4 = glm::inverse(tmpMat4);
    tmpMat4 = glm::transpose(tmpMat4);

    out[0].x = tmpMat4[0].x;
    out[0].y = tmpMat4[0].y;
    out[0].z = tmpMat4[0].z;

    out[1].x = tmpMat4[1].x;
    out[1].y = tmpMat4[1].y;
    out[1].z = tmpMat4[1].z;

    out[2].x = tmpMat4[2].x;
    out[2].y = tmpMat4[2].y;
    out[2].z = tmpMat4[2].z;
}

void Scene3d::liftUpVision(float ratio) {
    if (mCamera)
    {
        mCamera->liftUpVision(ratio);
        sendMessage(MessageId::MESSAGE_REQUEST_DRAW);
    }
}

void Scene3d::rotateModel(int angle) {
    if (mModel)
    {
        mModel->rotate(angle, 0.f, 1.f, 0.f);
        sendMessage(MessageId::MESSAGE_REQUEST_DRAW);
    }
}

