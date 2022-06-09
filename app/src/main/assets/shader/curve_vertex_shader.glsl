#version 300 es
precision highp float;
in vec4 aVertexCoords;

uniform float uPointSizePixel;
uniform mat4 uMatrix;

void main() {
    gl_PointSize = uPointSizePixel;
    gl_Position = uMatrix * vec4(aVertexCoords.x, aVertexCoords.y, 0.0, 1.0);
}
