#include "perspective_camera.h"

#include <glad/glad.h>

perspective_camera::perspective_camera(float fov, float width, float height, float near_clip, float far_clip) 
	: entity_object(mesh{}), camera_shader(shader::instantiate(UNLIT_SHADER_SOURCE))
{
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	projection_matrix = glm::perspective(glm::radians(fov), width / height, near_clip, far_clip);

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}

void perspective_camera::update_view_matrix()
{
	rotation_matrix = glm::mat4_cast(m_orientation);
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), m_position);

	view_matrix = translation_matrix * rotation_matrix;

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
}

void perspective_camera::set_projection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}