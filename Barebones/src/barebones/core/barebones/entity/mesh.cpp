#include "mesh.h"

#include <iostream>

mesh::mesh()
{

}

void mesh::set_vertices(std::vector<float> vertices, float vertex_size)
{
	data.vertices = vertices;
	data.vertex_size = vertex_size;
}

void mesh::set_indices(std::vector<uint32_t> indices, uint32_t index_size)
{
	data.indices = indices;
	data.index_size = index_size;
}