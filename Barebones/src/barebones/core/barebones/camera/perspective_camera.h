#pragma once

#include "barebones/rendering/shader/shader.h"

#include "barebones/core/barebones/entity/entity_object.h"

class perspective_camera : public entity_object
{
public:
	perspective_camera(float fov, float width, float height, float near_clip, float far_clip);

	void set_projection(float fov, float aspect_ratio, float near_clip, float far_clip);

	void set_position(const glm::vec3& position, const axis axis) override;
	void set_rotation(const glm::vec3& rotation, const axis axis) override;

	float get_fov() const { return m_fov; }
	float get_near_clip() const { return m_near_clip; }
	float get_far_clip() const { return m_far_clip; }

	const glm::mat4& get_view_matrix() const { return view_matrix; }
	const glm::mat4& get_projection_matrix() const { return projection_matrix; }
private:
	void update_view_matrix();
private:
	float m_fov = 45.0f;

	float m_near_clip = 0.1f;
	float m_far_clip = 50.0f;
private:
	std::shared_ptr<shader> camera_shader;

	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
};