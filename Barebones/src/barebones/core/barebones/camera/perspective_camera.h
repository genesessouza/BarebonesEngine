#pragma once

#include "barebones/rendering/shader/shader.h"

#include <glm/gtc/matrix_transform.hpp>

class perspective_camera
{
public:
	perspective_camera(float fov, float aspect_ratio, float near_clip, float far_clip, const char* shader_filepath);

	void set_position(const glm::vec3& position);
	const glm::vec3& get_position() const { return position; }

	void set_rotation(const glm::vec3& rotation);

	void set_projection(float fov, float aspect_ratio, float near_clip, float far_clip);

	const glm::mat4& get_view_matrix() const { return view_matrix; }
	const glm::mat4& get_projection_matrix() const { return projection_matrix; }
private:
	void update_view_matrix();
private:
	shader scene_shader;
	glm::vec3 position;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
};