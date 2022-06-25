//
// Created by lyzirving on 2022/6/20.
//
#include <GLES2/gl2.h>

#include "Scene3d.h"
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
                                     mModel(nullptr),
                                     mShader(nullptr),
                                     mViewM(1.f),
                                     mProjectionM(1.f) {}

Scene3d::~Scene3d() {
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
        uint32_t width = window->getWidth();
        uint32_t height = window->getHeight();
        glViewport(0, 0, width, height);

        mShader->use(true);

        mShader->setMat4(shader::view, mViewM);
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

