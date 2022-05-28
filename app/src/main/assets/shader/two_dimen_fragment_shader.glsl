precision mediump float;
uniform sampler2D uTwoDimensTextureSampler;
varying vec2 vTexCoords;

void main() {
    vec4 color = texture2D(uTwoDimensTextureSampler, vTexCoords);
    gl_FragColor = color;
}
