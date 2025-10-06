#pragma once

#include "array.h"
#include "buffer.h"

#include "barebones/core/barebones/entity/light.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

#include "barebones/rendering/shader/shader.h"

class frame_buffer 
{
public:
    frame_buffer(int width, int height);
    ~frame_buffer();

    int get_width() const { return m_width; }
    int get_height() const { return m_height; }
    
    void render_depth_map(light* light_source, perspective_camera* camera);
	void draw_screen_quad() const;

	const glm::mat4& get_light_view() const { return light_view_matrix; }
	const glm::mat4& get_light_proj() const { return light_proj_matrix; }
    const glm::mat4& get_light_space() const { return light_space_matrix; }

    unsigned int get() const { return m_renderer_id; }
    unsigned int get_depth_texture() const { return m_shadow_map_tex; }
public:
    const std::shared_ptr<vertex_array>& get_vao() const { return m_fbo_vao; }

    const std::shared_ptr<shader>& get_depth_shader() const { return m_depth_shader; }
    const std::shared_ptr<shader>& get_depth_debug_shader() const { return m_debug_shader; }
public:
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
private:
    glm::mat4 light_view_matrix;
    glm::mat4 light_proj_matrix;
	glm::mat4 light_space_matrix;
};