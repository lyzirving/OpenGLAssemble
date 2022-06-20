//
// Created by lyzirving on 2022/6/20.
//
#include "Scene3d.h"
#include "Model.h"
#include "FileSystem.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene3d"

Scene3d::Scene3d(const char *name) : RendererContext(name),
                                     mModel(nullptr) {}

Scene3d::~Scene3d() = default;

void Scene3d::draw() {}

bool Scene3d::onPrepare() {
    return true;
}

void Scene3d::onQuit() {}

