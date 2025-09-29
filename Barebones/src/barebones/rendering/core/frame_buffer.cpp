#include "frame_buffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

static std::vector<glm::vec3> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 inv = glm::inverse(proj * view);

	std::vector<glm::vec3> frustumCorners;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				glm::vec4 pt = inv * glm::vec4(
					2.0f * x - 1.0f,
					2.0f * y - 1.0f,
					2.0f * z - 1.0f,
					1.0f
				);
				frustumCorners.push_back(glm::vec3(pt) / pt.w);
			}
		}
	}
	return frustumCorners;
}

void frame_buffer::render_depth_map(light* light_source, perspective_camera* camera)
{
	glViewport(0, 0, get_width(), get_height());
	glBindFramebuffer(GL_FRAMEBUFFER, get());
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 light_dir = light_source->get_normal();

	glm::vec3 target = glm::vec3(0.0f);

	float distance = -20.0f;

	glm::vec3 light_pos = target - light_dir * distance;

	light_view_matrix = glm::lookAt(light_pos, target, glm::vec3(0.0f, 1.0f, 0.0f));

	std::vector<glm::vec3> frustumCorners = getFrustumCornersWorldSpace(camera->get_projection_matrix(), camera->get_view_matrix());

	for (auto& corner : frustumCorners) {
		glm::vec4 tr = light_view_matrix * glm::vec4(corner, 1.0f);
		corner = glm::vec3(tr);
	}

	glm::vec3 min = frustumCorners[0];
	glm::vec3 max = frustumCorners[0];

	for (int i = 1; i < frustumCorners.size(); i++) {
		min = glm::min(min, frustumCorners[i]);
		max = glm::max(max, frustumCorners[i]);
	}

	float margin = 50;

	glm::mat4 lightProjection = glm::ortho(
		min.x, max.x,
		min.y, max.y,
		min.z, max.z + margin // fix these somehow
	);

	light_proj_matrix = lightProjection;

	light_space_matrix = light_proj_matrix * light_view_matrix;

	get_depth_shader()->bind();
	get_depth_shader()->define_mat4("u_lightView", &light_view_matrix[0][0]);
	get_depth_shader()->define_mat4("u_lightProjection", &light_proj_matrix[0][0]);
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

void frame_buffer::draw_screen_quad() const
{
	get_debug_shader()->bind();
	get_debug_shader()->define_int("depthMap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, get_depth_texture());

	get_vao()->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}