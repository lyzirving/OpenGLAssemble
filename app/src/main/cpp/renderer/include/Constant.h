//
// Created by lyzirving on 2022/6/22.
//
#ifndef OPENGLASSEMBLE_CONSTANT_H
#define OPENGLASSEMBLE_CONSTANT_H

namespace shader {
    static constexpr const char* const model      = "uModel";
    static constexpr const char* const view       = "uView";
    static constexpr const char* const projection = "uProjection";
    static constexpr const char* const normalM = "uNormalMat3";
    static constexpr const char* const cameraPos = "uCameraPos";

    static constexpr const char* const lightPos = "uLight.position";
    static constexpr const char* const lightAmbientRgb = "uLight.ambientRgb";
    static constexpr const char* const lightDiffuseRgb = "uLight.diffuseRgb";
    static constexpr const char* const lightSpecularRgb = "uLight.specularRgb";
    static constexpr const char* const lightKa = "uLight.Ka";
    static constexpr const char* const lightKd = "uLight.Kd";
    static constexpr const char* const lightKs = "uLight.Ks";
    static constexpr const char* const lightShininess = "uLight.shininess";
}

namespace tex {
    static constexpr const char* const diffuse  = "texture_diffuse";
    static constexpr const char* const specular = "texture_specular";
}

#endif //OPENGLASSEMBLE_CONSTANT_H
