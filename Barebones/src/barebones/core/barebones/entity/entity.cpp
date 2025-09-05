#include "entity.h"

entity::entity(mesh mesh) : entity_object(mesh)
{
	entity_gizmo = new gizmo(GIZMO_UNLIT_SOURCE);

	update_gizmo();
}

void entity::update_gizmo()
{
	glm::vec3 center = getWorldCenter();

	entity_gizmo->set_position(center);
	entity_gizmo->set_rotation(get_rotation());
}
