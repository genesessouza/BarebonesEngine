#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_transform;
uniform mat4 u_lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

void main()
{
    vec4 worldPos = u_model * u_transform * vec4(aPos, 1.0);

    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(u_model))) * aNormal;

    FragPosLightSpace = u_lightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = u_projection * u_view * worldPos;
};

#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

out vec4 FragColor;

uniform vec3 u_lightDir;
uniform vec4 u_lightColor;
uniform vec4 u_color;

uniform sampler2D u_shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // NDC
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // fora do shadow map → sem sombra
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(u_shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    return (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-u_lightDir);

    // ambient
    vec3 ambient = 0.1 * u_lightColor.rgb;

    // diffuse
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * u_lightColor.rgb;

    // shadow
    float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 lighting = (ambient + (1.0 - shadow) + diffuse) * u_color.rgb;
    FragColor = vec4(lighting, u_color.a);
};
