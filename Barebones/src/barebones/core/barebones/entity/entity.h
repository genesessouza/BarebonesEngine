#pragma once

#include "entity_object.h"
#include "mesh.h"

#include "barebones/rendering/debug/gizmo.h"
#include "barebones/rendering/debug/bounds.h"

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

	const gizmo* expose_gizmo() { return entity_gizmo; }
	const bounds* expose_bounds() { return entity_bounds; }

	sdf_primitive expose_sdf_data() { return m_mesh.get_sdf_data(); }
private:
	glm::vec3 getWorldCenter() const { return glm::vec3(model_matrix * glm::vec4(m_mesh_bounds.local_center, 1.0f)); }
private:
	mesh m_mesh;

	gizmo* entity_gizmo;
	bounds* entity_bounds;

	mesh_bounds m_mesh_bounds;
};