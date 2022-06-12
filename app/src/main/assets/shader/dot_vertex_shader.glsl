#version 300 es

in vec3 aVertexAttribute;

uniform vec2 uResolution;
uniform float uPtSize;
uniform mat4 uMatrix;

vec2 transToCenter(float screenX, float screenY, vec2 resolution) {
    vec2 dst = vec2(screenX, screenY);
    dst.x = dst.x - resolution.x / 2.0;
    dst.y = resolution.y / 2.0 - dst.y;
    return dst;
}

void main() {
    vec2 pos = transToCenter(aVertexAttribute.x, aVertexAttribute.y, uResolution);
    pos.x = pos.x / (uResolution.x * 0.5);
    pos.y = pos.y / (uResolution.y * 0.5);

    gl_Position = uMatrix * vec4(pos.x, pos.y, 0.0, 1.0);
    gl_PointSize = uPtSize;
}
