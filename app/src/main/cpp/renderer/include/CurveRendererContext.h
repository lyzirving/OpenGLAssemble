//
// Created by lyzirving on 2022/6/8.
//
#ifndef OPENGLASSEMBLE_CURVERENDERERCONTEXT_H
#define OPENGLASSEMBLE_CURVERENDERERCONTEXT_H

#include <memory>

#include "RendererContext.h"

class CurveRenderer;

class CurveRendererContext : public RendererContext {
public:
    CurveRendererContext(const char* name);
    ~CurveRendererContext();

    virtual void draw() override;

protected:
    virtual bool onPrepare() override;
    virtual void onQuit() override;

private:
    std::shared_ptr<CurveRenderer> mCurveRenderer;
};

#endif //OPENGLASSEMBLE_CURVERENDERERCONTEXT_H
