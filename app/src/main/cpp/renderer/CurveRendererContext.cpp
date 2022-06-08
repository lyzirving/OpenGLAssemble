//
// Created by lyzirving on 2022/6/8.
//
#include <cstring>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "CurveRendererContext.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CurveRendererContext"

CurveRendererContext::CurveRendererContext(const char* name) : RendererContext(name) {}

CurveRendererContext::~CurveRendererContext() = default;

void CurveRendererContext::draw() {}

bool CurveRendererContext::onPrepare() {
    return true;
}

void CurveRendererContext::onQuit() {}

