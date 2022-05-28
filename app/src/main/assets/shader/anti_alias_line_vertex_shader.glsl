precision highp float;
attribute vec2 aVertexCoords;
attribute vec2 aTexCoords;
uniform mat4 uMatrix;
varying vec2 vTexCoords;

void main() {
    vTexCoords = vec2(aTexCoords.x, aTexCoords.y);
    gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);
}
