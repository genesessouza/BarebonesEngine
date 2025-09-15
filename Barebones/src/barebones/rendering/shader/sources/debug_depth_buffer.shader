#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D u_depthMap;
uniform float u_near;
uniform float u_far;

float linearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = texture(u_depthMap, TexCoords).r;
    float linear = linearizeDepth(depth, u_near, u_far) / u_far;
    FragColor = vec4(vec3(linear), 1.0);
}