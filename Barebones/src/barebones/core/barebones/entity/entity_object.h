
#pragma once

#include "mesh.h"

#include "material.h"

#include "barebones/rendering/core/array.h"

#include <glm/ext/matrix_transform.hpp>

class entity_object
{
public:
	entity_object(const mesh* mesh, const char* m_shader_filepath = nullptr);
	entity_object(const mesh& mesh, const char* m_shader_filepath = nullptr);
	entity_object(const char* m_shader_filepath = nullptr) : entity_object(mesh{}, m_shader_filepath) {}

	virtual void set_position(const glm::vec3& position) { this->position = position; update_model_matrix(); }
	virtual glm::vec3 get_position() const { return position; }

	virtual void set_rotation(const glm::vec3& rotation) { this->rotation = rotation; update_model_matrix(); }
	virtual glm::vec3 get_rotation() const { return rotation; }

	virtual void set_scale(const glm::vec3& scale) { this->scale = scale; update_model_matrix(); }
	virtual glm::vec3 get_scale() const { return scale; }

	virtual std::shared_ptr<material> get_material() const { return entity_material; }
	virtual std::shared_ptr<vertex_array> get_entity_vertex_array() const { return entity_vertex_array; }
	virtual std::shared_ptr<vertex_array> get_lines_vertex_array() const { return lines_vertex_array; }

	virtual glm::vec3 get_normal() const { return normal; }

	//virtual gizmo* expose_gizmo() const = 0;
	//virtual bounds* expose_bounds() const = 0;
private:
	void fill_faces(const mesh& mesh);

	struct edge
	{
		unsigned int a, b;
		bool operator==(const edge& other) const {
			return (a == other.a && b == other.b) || (a == other.b && b == other.a);
		}
	};
protected:
	void update_model_matrix();
protected:
	std::shared_ptr<vertex_array> entity_vertex_array;
	std::shared_ptr<vertex_buffer> entity_vertex_buffer;
	std::shared_ptr<index_buffer> entity_index_buffer;

	std::vector<edge> unique_edges;
	std::shared_ptr<vertex_array> lines_vertex_array;
	std::shared_ptr<index_buffer> lines_index_buffer;

	std::shared_ptr<material> entity_material;

	glm::mat4 model_matrix = glm::mat4(1.0f);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::vec3 base_normal = glm::vec3(1.0f);
	glm::vec3 normal = glm::vec3(1.0f);
};