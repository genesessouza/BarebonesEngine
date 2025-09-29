#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_lightSpaceMatrix;

uniform vec3 u_cameraPos;

out vec3 FragPos;
out vec3 Normal;
out vec3 cameraPos;
out vec4 FragPosLightSpace;

void main()
{
    vec4 worldPos = u_model * vec4(aPos, 1.0);

    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(u_model))) * aNormal;
    
    cameraPos = u_cameraPos;

    FragPosLightSpace = u_lightSpaceMatrix * worldPos;

    gl_Position = u_projection * u_view * worldPos;
};

#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 cameraPos;

in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

out vec4 FragColor;

uniform vec3 u_lightPos;
uniform vec3 u_lightDir;
uniform vec4 u_lightColor;
uniform vec4 u_color;
uniform bool u_directionalLight;

uniform sampler2D u_shadowMap;
uniform bool u_softShadows;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, float intensity)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if(projCoords.z <= 1.0)
    {
        projCoords = (projCoords + 1.0) / 2.0;
        
        float closestDepth = texture(u_shadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;

        float cosTheta = max(dot(lightDir, normalize(Normal)), 0.0);
        float bias = max(0.0005, 0.003 * (1.0 - cosTheta));

        return (currentDepth - bias > closestDepth) ? intensity : 0.0;
    }
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

vec2 randomOffset(vec2 uv, int x, int y) {
    float r1 = rand(uv + float(x) * 0.123);
    float r2 = rand(uv + float(y) * 0.456);
    return vec2(r1 - 0.5, r2 - 0.5);
}

float ShadowCalculationWithPCF(vec4 fragPosLightSpace, vec3 lightDir, float intensity)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if(projCoords.z <= 1.0)
    {
        projCoords = (projCoords + 1.0) / 2.0;
        
        float closestDepth = texture(u_shadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;

        float cosTheta = max(dot(lightDir, normalize(Normal)), 0.0);
        float bias = max(0.0005, 0.003 * (1.0 - cosTheta));

        float shadow = 0.0;

        int pcfCount = 1;

        vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
        for(int x = -pcfCount; x <= pcfCount; ++x) {
            for(int y = -pcfCount; y <= pcfCount; ++y) {
                vec2 jitter = randomOffset(projCoords.xy, x, y) * texelSize * 0.5;

                float pcfDepth = texture(u_shadowMap, projCoords.xy + (vec2(x, y) * texelSize) + jitter).r;
                shadow += projCoords.z - bias > pcfDepth ? intensity : 0.0;
            }
        }
        return shadow /= 9.0;
    }
}

uniform float u_ambientMultiplier;
uniform float u_diffuseMultiplier;
uniform float u_specularMultiplier;
uniform float u_shadowStrength;

void main()
{
    vec3 viewPos = normalize(cameraPos);
    
    vec3 normal = normalize(Normal);
    vec3 lightDir;

    if(u_directionalLight)
        lightDir = normalize(-u_lightDir);
    else
        lightDir = normalize(FragPos - u_lightPos);

    vec4 ambient = u_ambientMultiplier * u_lightColor;

    float diffuseStrength = max(dot(normal, lightDir), 0);
    vec4 diffuse = u_diffuseMultiplier * diffuseStrength * u_lightColor;

    float aa = u_specularMultiplier;

    vec4 specular = vec4(0);
    if(diffuse != vec4(0))
    {
        vec3 reflectDir = normalize(reflect(normal, lightDir));
        float specularStrength = pow(max(dot(viewPos, reflectDir), 0), 16);
        specular = specularStrength * u_lightColor;
    }

    float shadow = 0;
    
    if(u_softShadows)
        shadow = ShadowCalculationWithPCF(FragPosLightSpace, lightDir, u_shadowStrength);
    else
        shadow = ShadowCalculation(FragPosLightSpace, lightDir, u_shadowStrength);

    diffuse = diffuse * (1.0 - shadow);

    vec4 lighting = (ambient + diffuse + specular) * u_color;
    FragColor = lighting;

    // FragColor = u_lightColor;

    // FragColor = vec4(normal * 0.5 + 0.5, 1.0); 
};
