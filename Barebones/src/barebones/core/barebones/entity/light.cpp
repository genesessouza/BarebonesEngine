#include "light.h"

#include <glm/ext/vector_float3.hpp>

#include <iostream>

light::light(light_type type, const glm::vec3& color, const float& intensity) : entity(*new triangle(), LIT_SHADER_SOURCE)
{
	this->type = type;

	base_color = color;
	m_intensity = intensity;

	set_color(base_color, m_intensity);
}

light::~light() {}

void light::shine_on_objects(const entity* scene_entity_obj, const perspective_camera* camera)
{
	if (type == light_type::directional)
	{
		scene_entity_obj->get_material()->get_shader()->define_bool("u_directionalLight", true);
		scene_entity_obj->get_material()->get_shader()->define_vec3("u_lightDir", get_normal());
	}

	glm::vec3 light_pos = get_position();

	if (type == light_type::point)
	{
		float length = glm::length(get_position() - scene_entity_obj->get_position());

		m_intensity /= length;

		scene_entity_obj->get_material()->get_shader()->define_bool("u_directionalLight", false);
		scene_entity_obj->get_material()->get_shader()->define_vec3("u_lightPos", light_pos);
	}

	scene_entity_obj->get_material()->get_shader()->define_bool("u_softShadows", m_use_soft_shadows);
	scene_entity_obj->get_material()->get_shader()->define_vec4("u_lightColor", glm::vec4(get_color(), 1));
}