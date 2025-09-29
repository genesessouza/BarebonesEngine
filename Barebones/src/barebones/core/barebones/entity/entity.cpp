#include "entity.h"

entity::entity(mesh& mesh, const char* shader_filepath) : entity_object(mesh)
{
	if (shader_filepath == nullptr || std::strlen(shader_filepath) == 0)
		entity_material = material::instantiate(LIT_SHADER_SOURCE);
	else
		entity_material = material::instantiate(shader_filepath);

	entity_material->set_model_matrix(&model_matrix[0][0]);

	get_material()->set_shadow_strength(2.0f);
	get_material()->set_ambient(0.3f);
	get_material()->set_diffuse(0.7f);
	get_material()->set_specular(0.3f);
}