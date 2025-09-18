#include "frame_buffer.h"

#include <iostream>

frame_buffer::frame_buffer(int width, int height)
	: m_width(width), m_height(height)
{
	// CREATION BLOCK
	{
		glGenFramebuffers(1, &m_renderer_id);

		glGenTextures(1, &m_shadow_map_tex);
		glBindTexture(GL_TEXTURE_2D, m_shadow_map_tex);
	}

	// TEXTURE BLOCK
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float border_color[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color); // clamps color to max 1.0
	}

	// FINAL SETTINGS
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_map_tex, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbinds frame buffer after usage
	}

	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << status << std::endl;

	m_depth_shader = shader::instantiate(DEPTH_SHADER_SOURCE);
	m_debug_shader = shader::instantiate(DEPTH_DEBUG_SHADER_SOURCE);

	create_screen_quad();
}

frame_buffer::~frame_buffer()
{
	if (m_shadow_map_tex) {
		glDeleteTextures(1, &m_shadow_map_tex);
		m_shadow_map_tex = 0;
	}

	if (m_renderer_id) {
		glDeleteFramebuffers(1, &m_renderer_id);
		m_renderer_id = 0;
	}
}

void frame_buffer::create_screen_quad()
{
	float screen_quad[] = {
		// pos        // texCoords
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	m_fbo_vao = vertex_array::create();

	std::shared_ptr<vertex_buffer> vbo = vertex_buffer::create(screen_quad, sizeof(screen_quad));
	vbo->set_layout({ { shader_data_type::Float2, "aPos" }, { shader_data_type::Float2, "aTexCoords" } });

	m_fbo_vao->add_vertex_buffers(vbo);
}