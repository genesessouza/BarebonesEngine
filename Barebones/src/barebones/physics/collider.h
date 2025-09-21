#pragma once

#include "barebones/core/barebones/entity/mesh.h"

#include <glm/glm.hpp>
#include <iostream>

struct OBB
{
	glm::vec3 center;
	glm::vec3 halfSize;
	glm::vec3 axis[3];
};

class collider
{
public:
	collider(const float* vertices, size_t vertex_count, const glm::mat4& model_matrix) {}
	~collider();

	bool check_for_collision(const OBB& other);

	const OBB get_obb() const { return m_obb; }

	void update(const glm::mat4& matrix) 
	{ 
		m_model_matrix = matrix;

		//compute_obb(m_vertices, m_model_matrix);
	}
protected:
	std::vector<glm::vec3> extract_positions(const float* data, size_t vertex_count, size_t stride = 6);
	void compute_obb(const std::vector<glm::vec3>& vertices, const glm::mat4& modelMatrix);
private:
	std::pair<float, float> project_obb(const OBB& box, const glm::vec3& axis);

	bool overlap(const std::pair<float, float>& A, const std::pair<float, float>& B);
protected:
	OBB m_obb{};

	std::vector<glm::vec3> m_vertices;
	size_t m_vertex_count = 0;
	glm::mat4 m_model_matrix = glm::mat4(1.0f);
};

class box_collider : public collider
{
public:
	box_collider(const float* vertices, size_t vertex_count, const glm::mat4& model_matrix) : collider(vertices, vertex_count, model_matrix)
	{
		m_vertex_count = vertex_count; // first 3 floats (x, y, z) times 6 faces
		m_model_matrix = model_matrix;

		std::cout << "Vertex count: " << m_vertex_count << std::endl;
		std::cout << "Expected floats: " << m_vertex_count * 6 << std::endl;

		//m_vertices = extract_positions(vertices, m_vertex_count);

		//compute_obb(m_vertices, m_model_matrix);
	}
};