#version 320 es
precision mediump float;

#define REFLECTIVITY 64.0

in vec2 vTexCoords;
in vec3 vVertexWorldPos;
in vec3 vNormalWorld;

uniform sampler2D texture_diffuse1;

uniform vec3 uLightColor;
uniform vec3 uLightWorldPos;
uniform vec3 uCameraPos;

uniform float uAmbientCoefficient;
uniform vec3 uDiffuseCoefficient;
uniform vec3 uSpecularCoefficient;

out vec4 mFragColor;

void main() {
    vec4 texColor = texture(texture_diffuse1, vTexCoords);
    vec3 normal = normalize(vNormalWorld);

    vec3 ambient = uLightColor * uAmbientCoefficient;

    vec3 lightDir = normalize(uLightWorldPos - vVertexWorldPos);
    float cosThetaDiffuse = max(0.0, dot(lightDir, normal));
    vec3 diffuse = uLightColor * uDiffuseCoefficient * cosThetaDiffuse;

    vec3 visionDir = normalize(uCameraPos - vVertexWorldPos);
    vec3 bisector = normalize(visionDir + lightDir);
    float cosThetaSpecular = max(0.0, dot(bisector, normal));
    vec3 specular = uLightColor * uSpecularCoefficient * pow(cosThetaSpecular, REFLECTIVITY);

    //todo light will weaken by the increase of distance between light source and pixel
    vec3 resRgb = (ambient + diffuse + specular) * texColor.xyz;

    mFragColor = vec4(resRgb, texColor.w);
}
