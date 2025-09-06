#include "entity.h"

entity::entity(mesh mesh) : entity_object(mesh)
{
	entity_gizmo = new gizmo(DEBUG_SHADER_SOURCE);
	entity_bounds = new bounds(mesh, DEBUG_SHADER_SOURCE);

	glm::vec3 center = getWorldCenter();

	entity_gizmo->set_position(center);
	entity_gizmo->set_rotation(get_rotation());

	entity_bounds->set_position(center);
	entity_bounds->set_rotation(get_rotation());
}