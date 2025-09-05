#shader vertex
layout (location = 0) in vec3 aPos;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 u_color;

void main()
{
    FragColor = vec4(1.00, 0.71, 0.00, 1.0);
}