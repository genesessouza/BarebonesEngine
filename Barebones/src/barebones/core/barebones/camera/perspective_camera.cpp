#include "perspective_camera.h"

#include <glad/glad.h>

perspective_camera::perspective_camera(float fov, float width, float height, float near_clip, float far_clip)
	: entity_object(mesh{}), camera_shader(shader::instantiate(UNLIT_SHADER_SOURCE))
{
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	projection_matrix = glm::perspective(glm::radians(fov), width / height, near_clip, far_clip);

	m_fov = fov;
	m_near_clip = near_clip;
	m_far_clip = far_clip;

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}

void perspective_camera::set_projection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	m_fov = fov;
	m_near_clip = near_clip;
	m_far_clip = far_clip;

	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
	camera_shader->bind();
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}

void perspective_camera::set_position_and_rotation(const glm::vec3& position, const glm::vec3& rotation)
{
	m_position = position;

	m_rotation = glm::radians(rotation);

	glm::mat4 rx = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	glm::mat4 ry = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	glm::mat4 rz = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

	glm::mat4 delta = rz * ry * rx;

	m_orientation = glm::quat_cast(delta);

	update_view_matrix();
}

void perspective_camera::update_view_matrix()
{
	glm::mat4 rotation_matrix = glm::mat4_cast(m_orientation);
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), m_position);

	view_matrix = glm::inverse(rotation_matrix * translation_matrix);

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
}