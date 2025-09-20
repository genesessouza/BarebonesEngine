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
    Normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);

    FragPosLightSpace = u_lightSpaceMatrix * worldPos;

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

    if(projCoords.z <= 1.0)
    {
        projCoords = (projCoords + 1.0) / 2.0;
        
        float closestDepth = texture(u_shadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;

        // float bias = max(0.01f * (1.0f - dot(Normal, u_lightDir)), 0.0005f);
        // float bias = max(0.005f, 0.05f * (1.0 - dot(Normal, u_lightDir)));
        float bias = 0.005f;
        return (currentDepth > closestDepth + bias) ? 0.9 : 0.0;
    }
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = u_lightDir;

    vec3 ambient = 0.1 * u_lightColor.rgb;

    float diff = max(dot(norm, u_lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor.rgb;

    float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * u_color.rgb;
    FragColor = vec4(lighting, u_color.a);
};
