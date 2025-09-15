#include "renderer.h"


#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

renderer::scene_data* renderer::m_scene_data = new renderer::scene_data;

renderer::renderer(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

renderer::~renderer() {}

void renderer::begin_scene(const perspective_camera* camera, light* scene_light)
{
	m_scene_data->view_matrix = camera->get_view_matrix();
	m_scene_data->projection_matrix = camera->get_projection_matrix();

	m_scene_data->scene_light = scene_light;
}

void renderer::end_scene()
{
	//glDeleteFramebuffers(1, (GLuint*)m_scene_data->camera_frame_buffer->get());
	//glDeleteTextures(1, (GLuint*)m_scene_data->camera_frame_buffer->get_texture_color_buffer());
	//glDeleteRenderbuffers(1, (GLuint*)m_scene_data->camera_frame_buffer->get_rbo_depth_stencil());
}

void renderer::submit(const entity_object* entity_obj, const perspective_camera* scene_camera, const glm::mat4& transform)
{
	entity_obj->get_material()->get_shader()->bind();

	entity_obj->get_material()->get_shader()->define_mat4("u_view", &scene_camera->get_view_matrix()[0][0]);
	entity_obj->get_material()->get_shader()->define_mat4("u_projection", &scene_camera->get_projection_matrix()[0][0]);

	entity_obj->get_material()->get_shader()->define_mat4("u_transform", &transform[0][0]);

	glm::vec3 lightDir = glm::normalize(m_scene_data->scene_light->get_normal());

	glm::mat4 viewProj = scene_camera->get_view_matrix() * scene_camera->get_projection_matrix();
	glm::mat4 invViewProj = glm::inverse(viewProj);

	// Converte 8 cantos do frustum para world space
	std::vector<glm::vec3> frustumCorners;
	for (int x = -1; x <= 1; x += 2)
		for (int y = -1; y <= 1; y += 2)
			for (int z = -1; z <= 1; z += 2)
			{
				glm::vec4 corner = invViewProj * glm::vec4(x, y, z, 1.0f);
				frustumCorners.push_back(glm::vec3(corner) / corner.w);
			}

	// Calcula bounding box no espaço do mundo
	glm::vec3 center(0.0f);
	for (auto& v : frustumCorners)
		center += v;

	center /= frustumCorners.size();

	// Usa esse centro como alvo da luz
	glm::vec3 lightPos = center - lightDir * 20.0f;
	glm::mat4 lightView = glm::lookAt(lightPos, center, glm::vec3(0, 1, 0));
	glm::mat4 lightSpaceMatrix = scene_camera->get_projection_matrix() * lightView;

	entity_obj->get_material()->get_shader()->define_mat4("u_lightSpaceMatrix", &lightSpaceMatrix[0][0]);

	entity_obj->get_entity_vertex_array()->bind();

	draw(entity_obj);
}

void renderer::draw(const entity_object* entity_obj)
{
	glDrawElements(GL_TRIANGLES, entity_obj->get_entity_vertex_array()->get_index_buffer()->get_count(), GL_UNSIGNED_INT, 0);
}