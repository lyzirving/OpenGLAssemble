#version 320 es
precision mediump float;

in vec2 vTexCoords;
uniform sampler2D texture_diffuse1;
out vec4 mFragColor;

void main() {
    mFragColor = texture(texture_diffuse1, vTexCoords);
}
