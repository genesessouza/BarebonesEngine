#include "perspective_camera.h"

#include <glad/glad.h>

perspective_camera::perspective_camera(float fov, float aspect_ratio, float near_clip, float far_clip) : scene_shader(shader(LIT_SHADER_SOURCE)), position(glm::vec3(0.0f))
{
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);

	scene_shader.define_mat4("u_view", &view_matrix[0][0]);
	scene_shader.define_mat4("u_projection", &projection_matrix[0][0]);
}

void perspective_camera::set_position(const glm::vec3& position)
{
	this->position = position;
	view_matrix = glm::translate(glm::mat4(1.0f), this->position);
	update_view_matrix();
}

void perspective_camera::set_rotation(const glm::vec3& rotation)
{
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	update_view_matrix();
}

void perspective_camera::update_view_matrix()
{
	view_matrix = glm::inverse(view_matrix);
	scene_shader.define_mat4("u_view", &view_matrix[0][0]);
}

void perspective_camera::set_projection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
	scene_shader.define_mat4("u_projection", &projection_matrix[0][0]);
}