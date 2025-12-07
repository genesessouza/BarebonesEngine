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
    Normal = mat3(transpose(inverse(u_view))) * aNormal;

    gl_Position = u_projection * u_view * worldPos;
};


#shader fragment
#version 330 core

in vec3 Normal;
out vec4 FragColor;
in vec3 FragPos;

uniform vec4 u_color;

void main()
{
    vec3 norm = normalize(Normal);

    FragColor = vec4(u_color.rgba);
};