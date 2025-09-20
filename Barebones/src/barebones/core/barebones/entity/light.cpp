#include "light.h"

#include <glm/ext/vector_float3.hpp>

#include <iostream>

light::light(light_type type, const glm::vec3& color, const float& intensity) : entity(new triangle(), LIT_SHADER_SOURCE)
{
	this->base_color = color;
	this->type = type;

	set_color(color, intensity);
}

light::~light() {}

void light::shine_on_objects(const entity_object* scene_entity_obj, const perspective_camera* camera)
{
	float intensity = 1;
	if (type == light_type::directional)
	{
		intensity = 1;
		scene_entity_obj->get_material()->get_shader()->define_bool("u_directionalLight", true);
		scene_entity_obj->get_material()->get_shader()->define_vec3("u_lightDir", get_normal());
	}

	if (type == light_type::point)
	{
		float length = glm::length(get_position() - scene_entity_obj->get_position());

		intensity = intensity / length;
		scene_entity_obj->get_material()->get_shader()->define_bool("u_directionalLight", false);
		scene_entity_obj->get_material()->get_shader()->define_vec3("u_lightDir", get_position());
	}

	set_color(base_color, intensity);
	scene_entity_obj->get_material()->get_shader()->define_vec4("u_lightColor", glm::vec4(get_color(), 1));
}