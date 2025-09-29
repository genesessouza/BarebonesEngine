#pragma once

#include "entity_object.h"
#include "mesh.h"
#include "material.h"

class entity : public entity_object
{
public:
	entity(mesh& mesh, const char* shader_filepath);

	std::shared_ptr<material> get_material() const { return entity_material; }

	void set_static(const bool value) { is_static = value; }
	bool is_static_entity() const { return is_static; }
private:	
	bool is_static = false;
};