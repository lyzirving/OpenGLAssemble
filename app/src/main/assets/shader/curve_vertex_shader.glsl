#version 300 es
precision highp float;
in vec3 aVertexAttribute;

uniform vec2 uResolution;
uniform vec2 uStart;
uniform vec2 uControl;
uniform vec2 uEnd;
uniform float uLineWidth;
uniform mat4 uMatrix;

flat out vec2 vResolution;
flat out vec2 vStart;
flat out vec2 vControl;
flat out vec2 vEnd;
flat out float vLineWidth;

/*
 * transform the screen coordinate into a coordinate whose origin is the center of screen,
 * the dst coordinate' x axis directs to right, and y axis directs to up
 * screenX: x coordinate on screen
 * screenY: y coordinate on screen
 * resolution: screen's width and height
*/
vec2 transToCenter(float screenX, float screenY, vec2 resolution) {
    vec2 dst = vec2(screenX, screenY);
    dst.x = dst.x - resolution.x / 2.0;
    dst.y = resolution.y / 2.0 - dst.y;
    return dst;
}

float quadraticBessel(float t, vec2 start, vec2 control, vec2 end, bool measureX) {
    if(measureX) {
        return ((1.0 - t) * (1.0 - t) * start.x + 2.0 * t * (1.0 - t) * control.x + t * t * end.x);
    } else {
        return ((1.0 - t) * (1.0 - t) * start.y + 2.0 * t * (1.0 - t) * control.y + t * t * end.y);
    }
}

void main() {
    vResolution = uResolution;
    vStart = vec2(uStart.x, uResolution.y - uStart.y);
    vControl = vec2(uControl.x, uResolution.y - uControl.y);
    vEnd = vec2(uEnd.x, uResolution.y - uEnd.y);
    vLineWidth = uLineWidth;

    float t0 = aVertexAttribute.x;
    float t1 = aVertexAttribute.y;
    float side = aVertexAttribute.z;

    vec2 a = vec2(0.0, 0.0);
    vec2 b = vec2(0.0, 0.0);
    a.x = quadraticBessel(t0, uStart, uControl, uEnd, true);
    a.y = quadraticBessel(t0, uStart, uControl, uEnd, false);
    b.x = quadraticBessel(t1, uStart, uControl, uEnd, true);
    b.y = quadraticBessel(t1, uStart, uControl, uEnd, false);

    vec2 stdA = transToCenter(a.x, a.y, uResolution);
    vec2 stdB = transToCenter(b.x, b.y, uResolution);

    vec2 dirAB = normalize(stdB - stdA);
    vec2 nAB = vec2(-dirAB.y, dirAB.x);
    nAB = nAB * uLineWidth / 2.0 * side;

    vec2 dst = stdA + nAB;
    // transform the value to [-1, 1]
    dst.x = dst.x / (uResolution.x * 0.5);
    dst.y = dst.y / (uResolution.y * 0.5);

    gl_Position = uMatrix * vec4(dst.x, dst.y, 0.0, 1.0);
}
