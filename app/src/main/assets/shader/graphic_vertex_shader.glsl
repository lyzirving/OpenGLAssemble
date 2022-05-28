#version 320 es
precision mediump float;
in vec2 aVertexCoords;
uniform int uVertexCount;
uniform mat4 uMatrix;
uniform vec4 uGradient[2];
out vec4 vColor;

void main() {
    float rRange = uGradient[1].r - uGradient[0].r;
    float gRange = uGradient[1].g - uGradient[0].g;
    float bRange = uGradient[1].b - uGradient[0].b;
    float aRange = uGradient[1].a - uGradient[0].a;
    float ratio = float(gl_VertexID) / float(uVertexCount - 1);
    vColor = vec4(uGradient[0].r + ratio * rRange, uGradient[0].g + ratio * gRange,
                  uGradient[0].b + ratio * bRange, uGradient[0].a + ratio * aRange);
    gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);
}
