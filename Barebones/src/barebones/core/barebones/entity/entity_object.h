#pragma once

#include "mesh.h"
#include "material.h"

#include "barebones/rendering/core/array.h"

#include <glm/gtc/quaternion.hpp>

class entity_object
{
public:
	entity_object(const mesh* mesh, const char* shader_filepath = nullptr);
	entity_object(const mesh& mesh, const char* shader_filepath = nullptr);
	entity_object(const char* shader_filepath = nullptr) : entity_object(mesh{}, shader_filepath) {}

	virtual void set_position(const glm::vec3& position) { m_position = position; update_model_matrix(); }
	virtual glm::vec3 get_position() const { return m_position; }

	virtual void set_rotation(const glm::vec3& rotation) { m_orientation = glm::radians(rotation); update_model_matrix(); }
	virtual glm::vec3 get_rotation() const { return glm::degrees(glm::eulerAngles(m_orientation)); }

	virtual void set_scale(const glm::vec3& scale) { m_scale = scale; update_model_matrix(); }
	virtual glm::vec3 get_scale() const { return m_scale; }

	virtual std::shared_ptr<material> get_material() const { return entity_material; }
	virtual std::shared_ptr<vertex_array> get_entity_vertex_array() const { return entity_vertex_array; }

	virtual glm::vec3 get_normal() const { return normal; }
public:
	const glm::vec3 forward() const { return glm::normalize(glm::vec3(glm::mat4(1.0f) * glm::vec4(0, 0, -1, 0))); }
	const glm::vec3 right() const { return glm::normalize(glm::vec3(glm::mat4(1.0f) * glm::vec4(1, 0, 0, 0))); }
	const glm::vec3 up() const { return glm::normalize(glm::vec3(glm::mat4(1.0f) * glm::vec4(0, 1, 0, 0))); }
public:
	const glm::mat4& get_model_matrix() const { return model_matrix; }
private:
	void update_model_matrix();
protected:
	std::shared_ptr<vertex_array> entity_vertex_array;
	std::shared_ptr<vertex_buffer> entity_vertex_buffer;
	std::shared_ptr<index_buffer> entity_index_buffer;

	std::shared_ptr<material> entity_material;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 rotation_matrix = glm::mat4(1.0f);

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);

	glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 m_scale = glm::vec3(1.0f);

	glm::vec3 base_normal = glm::vec3(1.0f);
	glm::vec3 normal = glm::vec3(1.0f);
};