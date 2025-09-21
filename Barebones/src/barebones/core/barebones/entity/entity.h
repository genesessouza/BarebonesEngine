#pragma once

#include "entity_object.h"
#include "mesh.h"

#include "debug/gizmo.h"
#include "debug/bounds.h"

#include "barebones/physics/collider.h"

struct mesh_bounds
{
	glm::vec3 local_center;
	glm::vec3 local_min;
	glm::vec3 local_max;
};

class entity : public entity_object
{
public:
	entity(const mesh& mesh, const char* shader_filepath);

	gizmo* expose_gizmo() { return entity_gizmo; }
	bounds* expose_bounds() { return entity_bounds; }

	void set_position(const glm::vec3& new_pos) override { entity_object::set_position(new_pos); m_coll->update(model_matrix); }
	void set_rotation(const glm::vec3& new_rot) override { entity_object::set_rotation(new_rot); m_coll->update(model_matrix); }

	collider* get_collider() { return m_coll; }
private:
	glm::vec3 getWorldCenter() const { return glm::vec3(model_matrix * glm::vec4(m_mesh_bounds.local_center, 1.0f)); }
private:
	gizmo* entity_gizmo;
	bounds* entity_bounds;

	mesh_bounds m_mesh_bounds;
	collider* m_coll;
};