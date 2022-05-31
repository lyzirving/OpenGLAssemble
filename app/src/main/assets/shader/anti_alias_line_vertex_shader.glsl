#version 300 es
precision highp float;
in vec4 aVertexCoords;
in vec2 aTexCoords;
uniform mat4 uMatrix;
out vec2 vTexCoords;
//flat means no interpolation
flat out vec2 vTexPortSize;

void main() {
    vTexCoords = vec2(aTexCoords.x, aTexCoords.y);
    vTexPortSize = vec2(aVertexCoords.z, aVertexCoords.w);
    gl_Position = uMatrix * vec4(aVertexCoords.x, aVertexCoords.y, 0.0, 1.0);
}
