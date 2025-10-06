#include "collider.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <map>

collider::collider(mesh& mesh)
{
	extents_debug.mesh = mesh;

	glGenVertexArrays(1, &extents_debug.vao);
	glGenBuffers(1, &extents_debug.vbo);

	create_wireframe();

	extents_debug.debug_shader = shader::instantiate(DEBUG_SHADER_SOURCE);
	extents_debug.debug_shader->define_vec4("u_color", wireframe_color);
}

void collider::create_wireframe()
{
	std::vector<uint32_t> indices = extents_debug.mesh.get_indices();
	const std::vector<float>& vertices = extents_debug.mesh.get_vertices();

	int stride = 6; // 3 for position, 3 for normal

	std::map<std::pair<uint32_t, uint32_t>, int> edge_count;

	for (size_t i = 0; i < indices.size(); i += 3)
	{
		uint32_t p[3] = { indices[i], indices[i + 1], indices[i + 2] };
		for (int e = 0; e < 3; ++e)
		{
			uint32_t a = p[e];
			uint32_t b = p[(e + 1) % 3];

			// Prevents duplicates like (a,b) and (b,a)
			auto edge = std::minmax(a, b);
			edge_count[edge]++;
		}
	}

	// Edges that are only used once are boundary edges (i.e., part of the outline) and should be drawn
	for (const auto& [edge, count] : edge_count)
	{
		if (count == 1)
		{
			uint32_t a = edge.first;
			uint32_t b = edge.second;

			glm::vec3 v0(vertices[a * stride], vertices[a * stride + 1], vertices[a * stride + 2]);
			glm::vec3 v1(vertices[b * stride], vertices[b * stride + 1], vertices[b * stride + 2]);
		
			extents_debug.lines.emplace_back(v0, v1);
		}
	}

	set_wireframe_data();
}

void collider::set_wireframe_data()
{
	glBindVertexArray(extents_debug.vao);

	glBindBuffer(GL_ARRAY_BUFFER, extents_debug.vbo);
	glBufferData(GL_ARRAY_BUFFER, extents_debug.lines.size() * sizeof(glm::vec3), extents_debug.lines.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void collider::draw_collider(const glm::mat4& view_matrix, const glm::mat4& proj_matrix, const glm::mat4& model_matrix)
{
	extents_debug.debug_shader->bind();

	extents_debug.debug_shader->define_mat4("u_projection", &proj_matrix[0][0]);
	extents_debug.debug_shader->define_mat4("u_view", &view_matrix[0][0]);
	extents_debug.debug_shader->define_mat4("u_model", &model_matrix[0][0]);

	glBindVertexArray(extents_debug.vao);

	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, (GLsizei)extents_debug.lines.size() * 2);
	glLineWidth(1.0f);

	glBindVertexArray(0);
}