//
// Created by lyzirving on 2022/6/22.
//
#ifndef OPENGLASSEMBLE_CONSTANT_H
#define OPENGLASSEMBLE_CONSTANT_H

namespace shader {
    static constexpr const char* const model      = "uModel";
    static constexpr const char* const view       = "uView";
    static constexpr const char* const projection = "uProjection";
    static constexpr const char* const lightColor = "uLightColor";
    static constexpr const char* const lightWorldPos = "uLightWorldPos";
    static constexpr const char* const aCoefficient = "uAmbientCoefficient";
    static constexpr const char* const dCoefficient = "uDiffuseCoefficient";
    static constexpr const char* const sCoefficient = "uSpecularCoefficient";
    static constexpr const char* const normalM = "uNormalMat3";
    static constexpr const char* const cameraPos = "uCameraPos";
}

namespace tex {
    static constexpr const char* const diffuse  = "texture_diffuse";
    static constexpr const char* const specular = "texture_specular";
}

#endif //OPENGLASSEMBLE_CONSTANT_H
