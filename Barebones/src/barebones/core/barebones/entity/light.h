#pragma once

#include "barebones/core/barebones/entity/entity.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

enum class light_type
{
	directional,
	point
};

class light : public entity
{
public:
	light(light_type type, const glm::vec3& color, const float& intensity);
	~light();

	void shine_on_objects(const entity_object* scene_entity_obj, const perspective_camera* camera);

	void set_color(const glm::vec3& new_color, float intensity) { get_material()->set_color(glm::vec4(new_color, 1) * intensity); }
	const glm::vec3& get_color() const { return base_color; }

	void set_light_type(const light_type& new_type) { type = new_type; }
	light_type get_light_type() const { return type; }
private:
	glm::vec3 base_color;

	light_type type = light_type::directional;
};