#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_transform;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    vec4 worldPos = u_model * u_transform * vec4(aPos, 1.0);

    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(u_model))) * aNormal;

    gl_Position = u_projection * u_view * worldPos;
};

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 u_lightDir;
uniform vec4 u_lightColor;
uniform vec4 u_color;
uniform vec3 u_lightPos;

uniform bool u_directionalLight;
uniform bool u_quad;

// Definição do AABB do cubo
uniform vec3 u_cubeMin;
uniform vec3 u_cubeMax;

// Função de interseção raio vs AABB
bool intersectAABB(vec3 ro, vec3 rd, vec3 minB, vec3 maxB, float maxDist) {
    vec3 t1 = (minB - ro) / rd;
    vec3 t2 = (maxB - ro) / rd;

    vec3 tMin = min(t1, t2);
    vec3 tMax = max(t1, t2);

    float entry = max(max(tMin.x, tMin.y), tMin.z);
    float exit  = min(min(tMax.x, tMax.y), tMax.z);

    // interseção válida se entrada <= saída
    // e se a entrada está antes da luz
    return (entry < exit) && (entry > 0.0) && (entry < maxDist);
}

void main()
{
    if(u_quad)
    {
        if(u_directionalLight)
        {
            vec3 rd = normalize(u_lightDir);
            float maxDist = 100.0;

            bool blocked = intersectAABB(FragPos, rd, u_cubeMin, u_cubeMax, maxDist);

            vec3 norm = normalize(Normal);

            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * u_lightColor.xyz;

            // Se houver bloqueio, difusa = 0
            float diff = blocked ? 0.1 : max(dot(-rd, norm), 0.0);
            vec3 diffuse = diff * u_lightColor.xyz;

            vec3 result = (ambient + diffuse) * u_color.rgb;
            FragColor = vec4(result, u_color.a);
        }
        else
        {
            vec3 rd = normalize(u_lightPos - FragPos);
            float maxDist = length(u_lightPos - FragPos);

            bool blocked = intersectAABB(FragPos, rd, u_cubeMin, u_cubeMax, maxDist);

            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * u_lightColor.xyz;

            vec3 color = blocked ? vec3(0.1, 0.1, 0.1) : vec3(0.0, 1.0, 1.0);
            vec3 result = (ambient + color) * u_color.rgb * u_lightColor.xyz;
            FragColor = vec4(result, u_color.a);
        }
    }
    else
    {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(u_lightDir);

        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_lightColor.rgb;

        float diff = max(dot(lightDir, norm), 0.0);
        vec3 diffuse = diff * u_lightColor.rgb;

        vec3 result = (ambient + diffuse) * u_color.rgb;
        FragColor = vec4(result, u_color.a);

        // RAY CALCULATIONS (FOR SHADOW)
    
        // vec3 rayDir = normalize(u_lightPos - FragPos);
        // float rayLength = length(u_lightPos - FragPos);
    
        // vec3 debugColor = (rayDir * 0.5) + 0.5;
        // FragColor = vec4(debugColor, u_color.a);

        // ------------------------------------------ //
    
    }
};