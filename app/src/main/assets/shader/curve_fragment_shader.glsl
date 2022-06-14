#version 300 es
precision highp float;

uniform vec4 uColor;
uniform float uThreshold;

flat in vec2 vResolution;
flat in vec2 vStart;
flat in vec2 vControl;
flat in vec2 vEnd;
flat in float vLineWidth;

out vec4 mFragColor;

float det(vec2 a, vec2 b) {
    return a.x * b.y - b.x * a.y;
}

vec2 closestPointInSegment(vec2 a, vec2 b) {
    vec2 ab = b - a;
    return a + ab * clamp(-dot(a, ab) / dot(ab, ab), 0.0, 1.0);
}

//from: http://research.microsoft.com/en-us/um/people/hoppe/ravg.pdf
vec2 getDistanceVector(vec2 b0, vec2 b1, vec2 b2) {
    float a = det(b0, b2);
    float b = 2.0 * det(b1, b0);
    float d = 2.0 * det(b2, b1);

    if(abs(2.0 * a + b + d) < 1000.0)
        return closestPointInSegment(b0,b2);

    float f = b * d - a * a;
    vec2 d21 = b2 - b1;
    vec2 d10 = b1 - b0;
    vec2 d20 = b2 - b0;
    vec2 gf = 2.0 * (b * d21 + d * d10 + a * d20);
    gf = vec2(gf.y, -gf.x);
    vec2 pp = (-f) * gf / dot(gf, gf);
    vec2 d0p = b0 - pp;
    float ap = det(d0p, d20);
    float bp = 2.0 * det(d10, d0p);
    float t = clamp((ap + bp) / (2.0 * a + b + d), 0.0, 1.0);
    return mix(mix(b0, b1, t), mix(b1, b2, t), t);
}

/**
 * get distance between the points and center of bessel quadratic curve;
 * the result is [0, lineWidth * 0.5]
 */
float approxDistance(vec2 p, vec2 b0, vec2 b1, vec2 b2) {
    return length(getDistanceVector(b0-p, b1-p, b2-p));
}

void main() {
    /**
     * gl_FragCoord is a vec4-type variable;
     * gl_FragCoord is a coordinate based on the left-bottom of viewport;
     * the limit of gl_FragCoord is based on api glViewport(startX, startY, width, height);
     */
    vec2 posViewcoord = vec2(gl_FragCoord.x, gl_FragCoord.y);
    float dist = approxDistance(posViewcoord, vStart, vControl, vEnd);
    float thresholdVal = vLineWidth * 0.5 * uThreshold;
    float limit = vLineWidth * 0.5;
    if (dist < thresholdVal) {
        mFragColor = uColor;
    } else {
        float factor = (dist - thresholdVal) / (limit * (1.0 - uThreshold));
        mFragColor = vec4(uColor.r, uColor.g, uColor.b, smoothstep(0.0, 1.0, 1.0 - factor));
    }
}
