#include "bounds.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void bounds::fill_faces(const mesh& mesh)
{
	bounds_vertex_array = vertex_array::create();
	bounds_vertex_array->add_vertex_buffers(entity_vertex_buffer);

	std::vector<edge> unique_edges;

	//std::cout << "Bounds: index count = " << mesh.get_index_count() << std::endl;

	for (size_t i = 0; i < mesh.get_index_count(); i += 3)
	{
		unsigned int i0 = mesh.get_indices()[i];
		unsigned int i1 = mesh.get_indices()[i + 1];
		unsigned int i2 = mesh.get_indices()[i + 2];

		//std::cout << "Bounds: vertices [i0: " << i0 << "], [i1: " << i1 << "], [i2: " << i2 << "]" << std::endl;

		edge edges[3] = { {i0, i1}, {i1, i2}, {i2, i0} };

		for (auto& e : edges)
		{
			//std::cout << "Bounds: edges [a: " << e.a << "], [b: " << e.b << "]" << std::endl;
			auto it = std::find(unique_edges.begin(), unique_edges.end(), e);

			if (it != unique_edges.end())
				unique_edges.erase(it);
			else
				unique_edges.push_back(e);
		}

		// DEBUG
		//for (auto& e : edges)
		//	std::cout << "Bounds: edges --after [a: " << e.a << "], [b: " << e.b << "]" << std::endl;
	}

	std::vector<uint32_t> bounds_indices;
	bounds_indices.reserve(unique_edges.size());

	for (auto& e : unique_edges)
	{
		bounds_indices.push_back(e.a);
		bounds_indices.push_back(e.b);
	}

	// DEBUG
	//for (auto bi : bounds_indices)
	//	std::cout << "Bounds: bounds_indices [" << bi << "]" << std::endl;

	bounds_index_buffer = index_buffer::create(bounds_indices.data(), bounds_indices.size());
	bounds_vertex_array->set_index_buffer(bounds_index_buffer);
}

void bounds::render(const glm::mat4& view, const glm::mat4& proj)
{
	get_material()->get_debug_shader()->bind();

	get_material()->get_debug_shader()->define_mat4("u_view", &view[0][0]);
	get_material()->get_debug_shader()->define_mat4("u_projection", &proj[0][0]);
	get_material()->get_debug_shader()->define_vec3("u_color", default_color);

	bounds_vertex_array->bind();

	auto a = bounds_vertex_array->get_index_buffer()->get_count();

	//std::cout << a << std::endl;

	glLineWidth(1);
	glDrawElements(GL_LINES, a, GL_UNSIGNED_INT, nullptr);
	glLineWidth(1);
}