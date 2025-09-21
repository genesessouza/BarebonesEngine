#pragma once

#include <vector>
#include <cstdint>

#include <glm/gtc/matrix_transform.hpp>

struct mesh_data
{
	std::vector<float> vertices;

	std::vector<uint32_t> indices;

	size_t elements_per_vertex = 6; // position/normal
};

class mesh
{
public:
	mesh() = default;

	const std::vector<float>& get_vertices() const { return data.vertices; }
	void set_vertices(std::vector<float> vertices) { data.vertices = vertices; }

	std::vector<uint32_t> get_indices() const { return data.indices; }
	void set_indices(std::vector<uint32_t> indices) { data.indices = indices; }

	const size_t get_vertex_count() const { return data.vertices.size() / data.elements_per_vertex; }
	const size_t get_index_count() const { return data.indices.size(); }

	const size_t get_vertex_buffer_size() const { return data.vertices.size() * sizeof(float); }
	const size_t get_index_buffer_size() const { return data.indices.size() * sizeof(uint32_t); }
protected:
	mesh_data data;
};

class quad : public mesh
{
public:
	quad() : mesh()
	{
		std::vector<float> vertices = {
			-0.5f, 0.0f, -0.5f, 0, -1, 0,							// bottom-left		- 0
			-0.5f, 0.0f,  0.5f, 0, -1, 0,							// top-left			- 1
			 0.5f, 0.0f,  0.5f, 0, -1, 0,							// top-right		- 2
			 0.5f, 0.0f, -0.5f, 0, -1, 0							// bottom-right		- 3
		};
		std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

		set_vertices(vertices);
		set_indices(indices);
	}
};

class triangle : public mesh
{
public:
	triangle() : mesh()
	{
		std::vector<float> vertices = {
			-0.3f, -0.4f, 0.0f, 0, 0, 1,							// top			- 0
			 0.0f,  0.4f, 0.0f, 0, 0, 1,							// bottom-left	- 1
			 0.3f, -0.4f, 0.0f, 0, 0, 1								// bottom-right	- 2
		};
		std::vector<uint32_t> indices = { 0, 1, 2 };

		set_vertices(vertices);
		set_indices(indices);
	}
};

class cube : public mesh
{
public:
	cube() : mesh()
	{
		std::vector<float> vertices = {
			// front face
			-0.5f, 0.5f, -0.5f, 0, -1, 0,							// bottom-left-front		- 0		---------
			-0.5f, 0.5f,  0.5f, 0, -1, 0,							// top-left-front			- 1		|		|
			 0.5f, 0.5f,  0.5f, 0, -1, 0,							// top-right-front			- 2		|		|
			 0.5f, 0.5f, -0.5f, 0, -1, 0,							// bottom-right-front		- 3		---------

			 // back face
			 -0.5f, -0.5f,  0.5f, 0, 1, 0,							// bottom-left-back			- 4		---------
			 -0.5f, -0.5f, -0.5f, 0, 1, 0,							// top-left-back			- 5		|		|
			  0.5f, -0.5f, -0.5f, 0, 1, 0,							// top-right-back			- 6		|		|
			  0.5f, -0.5f,  0.5f, 0, 1, 0,							// bottom-right-back		- 7		---------

			  // top face
			  -0.5f,  0.5f, 0.5f, 0, 0, -1,							// top-left-front			- 8		---------
			  -0.5f, -0.5f, 0.5f, 0, 0, -1,							// top-left-back			- 9			
			   0.5f, -0.5f, 0.5f, 0, 0, -1,							// top-right-back			- 10
			   0.5f,  0.5f, 0.5f, 0, 0, -1,							// top-right-front			- 11

			   // bottom face
			   -0.5f, -0.5f, -0.5f, 0, 0, 1,						// top-left-front			- 12		
			   -0.5f,  0.5f, -0.5f, 0, 0, 1,						// top-left-back			- 13			
				0.5f,  0.5f, -0.5f, 0, 0, 1,						// top-right-back			- 14
				0.5f, -0.5f, -0.5f, 0, 0, 1,						// top-right-front			- 15	---------

				// left face
				-0.5f, -0.5f,  0.5f, 1, 0, 0,						// bottom-left-back			- 17			|
				-0.5f,  0.5f,  0.5f, 1, 0, 0,						// top-left-back			- 18			|
				-0.5f,  0.5f, -0.5f, 1, 0, 0,						// top-left-front			- 19			|
				-0.5f, -0.5f, -0.5f, 1, 0, 0,						// bottom-right-front		- 20			|

				// right face
				 0.5f, -0.5f, -0.5f, -1, 0, 0,						// bottom-left-back			- 21	|
				 0.5f,  0.5f, -0.5f, -1, 0, 0,						// top-left-back			- 22	|
				 0.5f,  0.5f,  0.5f, -1, 0, 0,						// top-left-front			- 23	|
				 0.5f, -0.5f,  0.5f, -1, 0, 0						// bottom-right-front		- 24	|
		};
		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0,										// front face 
			4, 5, 6, 6, 7, 4,										// back face 
			8, 9, 10, 10, 11, 8,									// top face 
			12, 13, 14, 14, 15, 12,									// bottom face 
			16, 17, 18, 18, 19, 16,									// left face 
			20, 21, 22, 22, 23, 20									// right face 
		};

		set_vertices(vertices);
		set_indices(indices);
	}
};

class sphere : public mesh
{
public:
	sphere() : mesh()
	{
	}
};