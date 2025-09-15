#include "frame_buffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

frame_buffer::frame_buffer(int width, int height) : m_width(width), m_height(height)
{
    // 1. Criar FBO
    glGenFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    // 2. Criar textura de profundidade
    glGenTextures(1, &m_depth_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT24, // formato interno
        width, height,
        0,
        GL_DEPTH_COMPONENT,   // formato dos dados
        GL_UNSIGNED_INT,      // tipo
        NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // 3. Anexar depth texture ao FBO
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        m_depth_texture,
        0
    );

    // 4. Desativar writes para color buffer (só profundidade)
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // 5. Checar status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    // 6. Desbindar
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}