#include "perspective_camera.h"

#include <glad/glad.h>

perspective_camera::perspective_camera(float fov, float width, float height, float near_clip, float far_clip) 
	: camera_shader(shader::instantiate(UNLIT_SHADER_SOURCE))
{
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	projection_matrix = glm::perspective(glm::radians(fov), width / height, near_clip, far_clip);

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}

void perspective_camera::update_view_matrix()
{
	//glm::vec3 rotation = glm::degrees(glm::eulerAngles(m_orientation));

	rotation_matrix =
		glm::rotate(glm::mat4(1.0f), glm::radians(-m_rotation.x), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-m_rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-m_rotation.z), glm::vec3(0, 0, 1));

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), -m_position) * rotation_matrix;

	view_matrix = glm::inverse(transform);

	camera_shader->define_mat4("u_view", &view_matrix[0][0]);
}

void perspective_camera::set_projection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
	camera_shader->define_mat4("u_projection", &projection_matrix[0][0]);
}