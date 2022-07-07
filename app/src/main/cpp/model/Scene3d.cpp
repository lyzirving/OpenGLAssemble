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
                                     mProjectionM(1.f) {}

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

        glClearColor(float(1), float(1), float(1), float(1));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        uint32_t width = window->getWidth();
        uint32_t height = window->getHeight();
        glViewport(0, 0, width, height);

        mShader->use(true);

        // View matrix specifies camera's position and sight direction in world coordinate,
        // When all vertices apply view matrix, they are transformed into camera coordinate.
        // In camera coordinate, camera is set at (0, 0, 0), and sight direction is parallel to (0, 0, -1).
        mShader->setMat4(shader::view, mCamera->getViewMatrix());

        // Cond 1: model will be set to center, and vertices will be scaled to [-1, 1] later.
        // Cond 2: camera's position is (0, 0, 3f) as default in world coordinate.
        // Cond 3: the distance from near clip plane to camera is 1.
        // According to the three conditions above, in camera coordinate, camera is set at (0, 0, 0).
        // Near clip plane is set at (0, 0, -1).
        // Model's center is at (0, 0, -3), and the z value of model vertices ranges in [-2, -4].
        // Thus all vertices of the model are visible for this perspective projection.
        mProjectionM = glm::perspective(glm::radians(mCamera->getViewFieldY()),
                                        float(width) / float(height),
                                        1.f, (1.f + 2 * 1.5f));
        // h = tan[(field of view in y) * 0.5] * near, bottom = -h
        // left = -aspect ratio * h, right = -left
        mShader->setMat4(shader::projection, mProjectionM);

        mModel->draw(mShader);

        mShader->use(false);

        window->swapBuffer();
        it++;
    }
}

bool Scene3d::onPrepare() {
    mShader = std::make_shared<Shader>("shader/model_vs.glsl", "shader/model_fs.glsl");
    if(!mShader->valid())
        goto fail;

    mModel = std::make_shared<Model>(FileSystem::getPath("nanosuit/nanosuit.obj").c_str());
    return true;

    fail:
    return false;
}

void Scene3d::onQuit() {
    mModel.reset();
    mShader.reset();
}

