#pragma once

#include "entity_object.h"
#include "mesh.h"

#include "debug/gizmo.h"
#include "debug/bounds.h"

struct mesh_bounds
{
	glm::vec3 local_center;
	glm::vec3 local_min;
	glm::vec3 local_max;
};

class entity : public entity_object
{
public:
	entity(mesh* mesh, const char* shader_filepath);

	gizmo* expose_gizmo() { return entity_gizmo; }
	bounds* expose_bounds() { return entity_bounds; }
private:
	glm::vec3 getWorldCenter() const { return glm::vec3(model_matrix * glm::vec4(m_mesh_bounds.local_center, 1.0f)); }
private:
	gizmo* entity_gizmo;
	bounds* entity_bounds;

	mesh_bounds m_mesh_bounds;
};