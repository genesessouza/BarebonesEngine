#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>

class frame_buffer {
public:
    frame_buffer(int width, int height);
    ~frame_buffer();

    void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id); }
    void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void bind_for_writing() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
        glViewport(0, 0, m_width, m_height);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    GLuint get() const { return m_renderer_id; }
    GLuint get_depth_texture() const { return m_depth_texture; }
    int get_width() const { return m_width; }
    int get_height() const { return m_height; }

    void resize(int width, int height);

private:
    GLuint m_renderer_id = 0;
    GLuint m_depth_texture = 0;

    int m_width = 0;
    int m_height = 0;
};