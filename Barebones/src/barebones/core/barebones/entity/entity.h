#pragma once

#include "entity_object.h"
#include "mesh.h"

#include <barebones/rendering/debug/gizmo.h>

struct mesh_bounds
{
	glm::vec3 local_center;
	glm::vec3 local_min;
	glm::vec3 local_max;
};

class entity : public entity_object
{
public:
	entity(mesh mesh);

	void update_gizmo();

	const gizmo* expose_gizmo() { return entity_gizmo; }
private:
	glm::vec3 getWorldCenter() const { return glm::vec3(model_matrix * glm::vec4(m_mesh_bounds.local_center, 1.0f)); }
private:
	gizmo* entity_gizmo;
	mesh_bounds m_mesh_bounds;
};