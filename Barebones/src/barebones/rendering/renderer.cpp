#include "renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

renderer::scene_data* renderer::m_scene_data = new renderer::scene_data;

renderer::renderer(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

renderer::~renderer() {}

void renderer::begin_scene(const perspective_camera* camera)
{
	m_scene_data->view_matrix = camera->get_view_matrix();
	m_scene_data->projection_matrix = camera->get_projection_matrix();
}

void renderer::submit(const entity_object* entity_obj, const perspective_camera* scene_camera, const glm::mat4& transform)
{
	entity_obj->get_material()->get_shader()->bind();

	entity_obj->get_material()->get_shader()->define_mat4("u_view", &scene_camera->get_view_matrix()[0][0]);
	entity_obj->get_material()->get_shader()->define_mat4("u_projection", &scene_camera->get_projection_matrix()[0][0]);

	entity_obj->get_material()->get_shader()->define_mat4("u_transform", &transform[0][0]);

	entity_obj->get_entity_vertex_array()->bind();

	draw(entity_obj);

	entity_obj->get_material()->get_debug_shader()->bind();

	entity_obj->get_material()->get_debug_shader()->define_mat4("u_view", &scene_camera->get_view_matrix()[0][0]);
	entity_obj->get_material()->get_debug_shader()->define_mat4("u_projection", &scene_camera->get_projection_matrix()[0][0]);

	entity_obj->get_lines_vertex_array()->bind();

	draw_debug(entity_obj);
}

void renderer::draw(const entity_object* entity_obj)
{
	glDrawElements(GL_TRIANGLES, entity_obj->get_entity_vertex_array()->get_index_buffer()->get_count(), GL_UNSIGNED_INT, 0);
}

void renderer::draw_debug(const entity_object* entity_obj)
{
	uint32_t index_count = entity_obj->get_lines_vertex_array()->get_index_buffer()->get_count();
	glLineWidth(1.5);
	glDrawElements(GL_LINES, index_count, GL_UNSIGNED_INT, 0);
	glLineWidth(1);
}