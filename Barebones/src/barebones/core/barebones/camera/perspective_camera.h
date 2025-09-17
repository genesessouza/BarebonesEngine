#pragma once

#include "barebones/rendering/shader/shader.h"

#include "barebones/core/barebones/entity/entity_object.h"

class perspective_camera : public entity_object
{
public:
	perspective_camera(float fov, float width, float height, float near_clip, float far_clip);

	void set_projection(float fov, float aspect_ratio, float near_clip, float far_clip);

	void set_position(const glm::vec3& position) override { m_position = position; update_view_matrix(); }
	void set_rotation(const glm::vec3& rotation) override { m_rotation = rotation; update_view_matrix(); }

	const glm::mat4& get_view_matrix() const { return view_matrix; }
	const glm::mat4& get_projection_matrix() const { return projection_matrix; }
private:
	void update_view_matrix();
private:
	std::shared_ptr<shader> camera_shader;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
};