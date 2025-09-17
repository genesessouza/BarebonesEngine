#pragma once

#include "barebones/core/barebones/entity/mesh.h"
#include "barebones/core/barebones/entity/entity_object.h"
#include "barebones/rendering/core/array.h"

#include <memory>
#include <vector>

//////////////////////////////////////// TODO ///////////////////////////////////////////////
//////////////////////////////////// OPTIMIZE THIS SHYT /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

class bounds : public entity_object
{
public:
	bounds(const mesh* mesh, const char* shader_filepath)
		: entity_object(mesh, shader_filepath)
	{
		fill_faces(*mesh);
	}

	void render(const glm::mat4& view, const glm::mat4& proj);
private:
	void fill_faces(const mesh& mesh);
public:
	struct edge
	{
		unsigned int a, b;
		bool operator==(const edge& other) const {
			return (a == other.a && b == other.b) || (a == other.b && b == other.a);
		}
	};
private:
	std::vector<edge> unique_edges;
	std::shared_ptr<vertex_array> bounds_vertex_array;
	std::shared_ptr<index_buffer> bounds_index_buffer;
private:
	glm::vec3 default_color = glm::vec3(1.00, 0.71, 0.00); // light_orange
};