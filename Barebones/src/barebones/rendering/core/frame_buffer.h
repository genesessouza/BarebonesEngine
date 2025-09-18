#pragma once

#include "array.h"
#include "buffer.h"

#include "barebones/rendering/shader/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>

class frame_buffer {
public:
    frame_buffer(int width, int height);
    ~frame_buffer();

    unsigned int get() const { return m_renderer_id; }
    unsigned int get_depth_texture() const { return m_shadow_map_tex; }
    
    int get_width() const { return m_width; }
    int get_height() const { return m_height; }
public:
    const std::shared_ptr<vertex_array>& get_vao() const { return m_fbo_vao; }

    const std::shared_ptr<shader>& get_depth_shader() const { return m_depth_shader; }
    const std::shared_ptr<shader>& get_debug_shader() const { return m_debug_shader; }
private:
    void create_screen_quad();
private:
    unsigned int m_renderer_id;
    unsigned int m_shadow_map_tex;

    int m_width = 0;
    int m_height = 0;
private:
    std::shared_ptr<shader> m_depth_shader;
    std::shared_ptr<shader> m_debug_shader;

    std::shared_ptr<vertex_array> m_fbo_vao;
};