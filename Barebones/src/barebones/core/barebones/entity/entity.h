#pragma once

#include "entity_object.h"
#include "mesh.h"
#include "material.h"

class entity : public entity_object
{
public:
	entity(mesh& mesh, const char* shader_filepath);

	void set_position(const glm::vec3& new_pos) override { entity_object::set_position(new_pos); update_model_matrix(); }
	void set_rotation(const glm::vec3& new_rot) override { entity_object::set_rotation(new_rot); update_model_matrix(); }

	std::shared_ptr<material> get_material() const { return entity_material; }

	void set_static(const bool value) { is_static = value; }
	bool is_static_entity() const { return is_static; }
private:
	void update_model_matrix() override { entity_object::update_model_matrix(); entity_material->set_model_matrix(&model_matrix[0][0]); }
private:
	std::shared_ptr<material> entity_material;
	
	bool is_static = false;
};