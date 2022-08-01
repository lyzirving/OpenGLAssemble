#version 320 es
precision mediump float;

struct Light {
    vec3 position;
    vec3 Ka, Kd, Ks;
    float Ia, Id, Is;
    float shininess;
};

in vec2 vTexCoords;
in vec3 vVertexWorldPos;
in vec3 vNormalWorld;

uniform sampler2D uSamplerDiffuse;
uniform Light uLight;
uniform vec3 uCameraPos;

out vec4 mFragColor;

void main() {
    vec4 texColor = texture(uSamplerDiffuse, vTexCoords);
    vec3 normal = normalize(vNormalWorld);

    vec3 lightAmbient = uLight.Ka * uLight.Ia;

    vec3 lightDir = normalize(uLight.position - vVertexWorldPos);
    float cosThetaDiffuse = max(0.0, dot(lightDir, normal));
    vec3 lightDiffuse = uLight.Kd * uLight.Id * cosThetaDiffuse;

    vec3 visionDir = normalize(uCameraPos - vVertexWorldPos);
    vec3 bisector = normalize(visionDir + lightDir);
    float cosThetaSpecular = max(0.0, dot(bisector, normal));
    vec3 lightSpecular = uLight.Ks * uLight.Is * pow(cosThetaSpecular, uLight.shininess);

    //todo light will weaken by the increase of distance between light source and pixel
    vec3 fragRgb = (lightAmbient + lightDiffuse + lightSpecular) * texColor.xyz;

    mFragColor = vec4(fragRgb, texColor.w);
}
