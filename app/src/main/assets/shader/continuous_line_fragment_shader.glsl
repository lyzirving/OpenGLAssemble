#version 300 es
precision highp float;

uniform vec4 uColor;
uniform float uThreshold;

in vec2 vTexCoords;
//flat means no interpolation
flat in vec2 vTexPortSize;

out vec4 mFragColor;

vec2 transform(vec2 origin, vec2 viewport) {
    vec2 pos = vec2(origin.x, origin.y);
    if (viewport.x >= viewport.y) {
        pos.x *= viewport.x / viewport.y;
    } else {
        pos.y *= viewport.y / viewport.x;
    }
    return pos;
}

float distToLine(vec2 pt, vec2 start, vec2 end) {
    vec2 pos = pt - start;
    vec2 dir = end - start;
    float len = length(dir);
    dir /= len;
    float f = dot(dir, pos);
    if (f < 0.0)
    return distance(start, pt);
    else if (f > len)
    return distance(end, pt);
    else
    return distance(start + dir * f, pt);
}

void main() {
    vec2 pos = transform(vTexCoords, vTexPortSize);
    vec2 horLeftPt = transform(vec2(0.0, 0.5), vTexPortSize);
    vec2 horRightPt = transform(vec2(1.0, 0.5), vTexPortSize);

    float distToHorLine = distToLine(pos, horLeftPt, horRightPt);
    float threshold = clamp(uThreshold, 0.3, 0.5);

    if (distToHorLine <= threshold) {
        mFragColor = uColor;
    } else {
        float factor = (distToHorLine - threshold) / (0.5 - threshold);
        mFragColor = vec4(uColor.r, uColor.g, uColor.b, smoothstep(0.0, 1.0, 1.0 - factor));
    }
}
