#include "entity.h"

entity::entity(const mesh& mesh, const char* shader_filepath) : entity_object(mesh, shader_filepath)
{
	entity_material->set_shader(shader::instantiate(shader_filepath));

	entity_gizmo = new gizmo(DEBUG_SHADER_SOURCE);
	entity_bounds = new bounds(mesh, DEBUG_SHADER_SOURCE);

	glm::vec3 center = getWorldCenter();

	entity_gizmo->set_position(center);
	entity_gizmo->set_rotation(get_rotation());

	entity_bounds->set_position(center);
	entity_bounds->set_rotation(get_rotation());

	m_coll = new box_collider(entity_vertex_buffer->get_vertices(), mesh.get_vertex_count(), model_matrix);
}