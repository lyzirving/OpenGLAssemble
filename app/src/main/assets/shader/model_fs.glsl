#version 320 es
precision mediump float;

in vec2 vTexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 uLightColor;
uniform float uAmbientCoefficient;

out vec4 mFragColor;

void main() {
    // the anotated code will draw the depth graph
    //mFragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    vec3 ambient = uLightColor * uAmbientCoefficient;
    vec4 texColor = texture(texture_diffuse1, vTexCoords);
    vec3 resRgb = texColor.xyz * ambient;

    mFragColor = vec4(resRgb, texColor.w);
}
