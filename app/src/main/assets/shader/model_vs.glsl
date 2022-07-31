#version 320 es
precision mediump float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMat3;

out vec2 vTexCoords;
out vec3 vVertexWorldPos;
out vec3 vNormalWorld;

void main() {
    vTexCoords = aTexCoords;
    vVertexWorldPos = vec3(uModel * vec4(aPos, 1.0));
    vNormalWorld = uNormalMat3 * aNormal;

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
