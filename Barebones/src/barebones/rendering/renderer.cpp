#include "renderer.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

renderer::scene_data* renderer::m_scene_data = new renderer::scene_data;

renderer::renderer(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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
}

void renderer::submit(const entity_object* entity_obj, const glm::mat4& light_space_matrix, const frame_buffer* fbo)
{
	entity_obj->get_material()->get_shader()->bind();

	entity_obj->get_material()->get_shader()->define_mat4("u_view", &m_scene_data->view_matrix[0][0]);
	entity_obj->get_material()->get_shader()->define_mat4("u_projection", &m_scene_data->projection_matrix[0][0]);

	entity_obj->get_material()->get_shader()->define_mat4("u_transform", &glm::mat4(1.0f)[0][0]);

	// SHADOW MAPPING
	{
		entity_obj->get_material()->get_shader()->define_mat4("u_lightSpaceMatrix", &light_space_matrix[0][0]);
		entity_obj->get_material()->get_shader()->define_int("u_shadowMap", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->get_depth_texture());
	}

	entity_obj->get_entity_vertex_array()->bind();

	draw(entity_obj);
}

void renderer::submit_depth(const entity_object* entity_obj, const glm::mat4& light_view, const glm::mat4& light_proj)
{
	entity_obj->get_material()->get_shader()->bind();

	entity_obj->get_material()->get_shader()->define_mat4("u_view", &light_view[0][0]);
	entity_obj->get_material()->get_shader()->define_mat4("u_projection", &light_proj[0][0]);

	entity_obj->get_material()->get_shader()->define_mat4("u_transform", &glm::mat4(1.0f)[0][0]);

	entity_obj->get_entity_vertex_array()->bind();

	draw(entity_obj);
}

void renderer::draw(const entity_object* entity_obj)
{
	glDrawElements(GL_TRIANGLES, entity_obj->get_entity_vertex_array()->get_index_buffer()->get_count(), GL_UNSIGNED_INT, 0);
}