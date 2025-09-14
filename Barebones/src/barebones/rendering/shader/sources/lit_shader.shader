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

    FragPos = vec3(worldPos);
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

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-u_lightDir);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor.rgb;

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * u_lightColor.rgb;

    vec3 result = (ambient + diffuse) * u_color.rgb;

    FragColor = vec4(result, u_color.a);
};
