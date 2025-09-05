#pragma once

#include <vector>
#include <cstdint>

struct mesh_data
{
	std::vector<float> vertices;
	float vertex_size;

	std::vector<uint32_t> indices;
	uint32_t index_size;
};

class mesh
{
public:
	mesh();

	const std::vector<float>& get_vertices() const { return data.vertices; }
	void set_vertices(std::vector<float> vertices, float vertex_size);

	std::vector<uint32_t> get_indices() const { return data.indices; }
	void set_indices(std::vector<uint32_t> indices, uint32_t index_size);

	float get_vertex_size() const { return data.vertex_size; }
	uint32_t get_indices_count() const { return data.index_size; }
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
		float vertex_size = sizeof(float) * 24;						// 4 vertices, each with 6 floats (position + normal) = 24
		std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
		uint32_t index_size = 6;									// 2 triangles, each with 3 indices = 6

		set_vertices(vertices, vertex_size);
		set_indices(indices, index_size);
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
		float vertex_size = sizeof(float) * 18;						// 3 vertices, each with 6 floats (position + normal) = 18
		std::vector<uint32_t> indices = { 0, 2, 1 };
		uint32_t index_size = 3;									// 1 triangle, with 3 indices = 3

		set_vertices(vertices, vertex_size);
		set_indices(indices, index_size);
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
		float vertex_size = sizeof(float) * 144;					// 24 vertices, each with 6 floats (position + normal) = 144
		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0,										// front face 
			4, 5, 6, 6, 7, 4,										// back face 
			8, 9, 10, 10, 11, 8,									// top face 
			12, 13, 14, 14, 15, 12,									// bottom face 
			16, 17, 18, 18, 19, 16,									// left face 
			20, 21, 22, 22, 23, 20									// right face 
		};
		uint32_t index_size = 36;									// 12 triangles, each with 3 indices = 36

		set_vertices(vertices, vertex_size);
		set_indices(indices, index_size);
	}
};