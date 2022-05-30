#version 300 es
precision highp float;
in vec2 aVertexCoords;
in vec2 aTexCoords;
uniform mat4 uMatrix;
out vec2 vTexCoords;

void main() {
    vTexCoords = vec2(aTexCoords.x, aTexCoords.y);
    gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);
}
