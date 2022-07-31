#version 320 es
precision mediump float;

in vec2 vTexCoords;
in vec3 vVertexWorldPos;
in vec3 vNormalWorld;

uniform sampler2D texture_diffuse1;

uniform vec3 uLightColor;
uniform vec3 uLightWorldPos;

uniform float uAmbientCoefficient;
uniform vec3 uDiffuseCoefficient;

out vec4 mFragColor;

void main() {
    vec4 texColor = texture(texture_diffuse1, vTexCoords);

    vec3 ambient = uLightColor * uAmbientCoefficient;

    vec3 lightDir = normalize(uLightWorldPos - vVertexWorldPos);
    vec3 normal = normalize(vNormalWorld);
    float cosTheta = max(0.0, dot(lightDir, normal));
    vec3 diffuse = uLightColor * uDiffuseCoefficient * cosTheta;

    //todo light will weaken by the increase of distance between light source and pixel
    vec3 resRgb = (ambient + diffuse) * texColor.xyz;

    mFragColor = vec4(resRgb, texColor.w);
}
