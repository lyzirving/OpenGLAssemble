precision highp float;
uniform vec4 uColor;
uniform vec2 uViewport;
uniform float uVerticalThreshold;
uniform float uLeftEndPtPos;
varying vec2 vTexCoords;

vec2 transform(vec2 origin, vec2 viewport) {
    vec2 pos = vec2(origin.x, origin.y);
    if (viewport.x >= viewport.y) {
        pos.x *= viewport.x / viewport.y;
    } else {
        pos.y *= viewport.y / viewport.x;
    }
    return pos;
}

bool inCircle(vec2 pt, vec2 origin, float radius) {
    float dist = distance(pt, origin);
    return dist <= radius;
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
    vec2 pos = transform(vTexCoords, uViewport);
    vec2 horLeftPt = transform(vec2(0.0, 0.5), uViewport);
    vec2 horRightPt = transform(vec2(1.0, 0.5), uViewport);

    vec2 leftEndPos = transform(vec2(uLeftEndPtPos, 0.5), uViewport);
    vec2 rightEndPos = transform(vec2(1.0 - uLeftEndPtPos, 0.5), uViewport);

    float dist = distToLine(pos, horLeftPt, horRightPt);
    float threshold = clamp(uVerticalThreshold, 0.3, 0.5);
    if (dist <= threshold) {
        gl_FragColor = uColor;
    } else {
        float factor = (dist - threshold) / (0.5 - threshold);
        gl_FragColor = vec4(uColor.r, uColor.g, uColor.b, smoothstep(0.0, 1.0, 1.0 - factor));
    }
}
