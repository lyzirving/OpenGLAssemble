#version 320 es
precision mediump float;

in vec2 vTexCoords;
uniform sampler2D texture_diffuse1;
out vec4 mFragColor;

void main() {
    // the anotated code will draw the depth graph
    //mFragColor = vec4(vec3(gl_FragCoord.z), 1.0f);

    mFragColor = texture(texture_diffuse1, vTexCoords);
}
