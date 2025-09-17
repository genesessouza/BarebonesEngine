#include "frame_buffer.h"

#include <iostream>

frame_buffer::frame_buffer(int width, int height)
    : m_width(width), m_height(height)
{
    // 1) FBO
    glGenFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    // 2) Depth texture
    glGenTextures(1, &m_depth_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);

    // Internal format: usar 24 bits é um bom trade-off; pode usar 32 se precisar de mais precisão.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

    // Filtragem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Wrapping com border (útil para evitar artefatos fora do frustum)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Se você quer amostrar com sampler2D (comparação manual no shader), garanta GL_TEXTURE_COMPARE_MODE = GL_NONE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    // 3) Anexa só profundidade ao FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

    // 4) Desativa buffers de cor (depth-only FBO)
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // 5) Checagem
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: Depth framebuffer not complete!" << std::endl;

    // 6) Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

frame_buffer::~frame_buffer()
{
    if (m_depth_texture) {
        glDeleteTextures(1, &m_depth_texture);
        m_depth_texture = 0;
    }
    if (m_renderer_id) {
        glDeleteFramebuffers(1, &m_renderer_id);
        m_renderer_id = 0;
    }
}

void frame_buffer::resize(int width, int height)
{
    if (width == m_width && height == m_height) return;
    m_width = width;
    m_height = height;

    // Recria a textura de profundidade com o novo tamanho
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}