#pragma once

#include "barebones/core/entry.h"
#include "barebones/barebones.h"

class physics_sample : public application
{
public:
	physics_sample();
private:
	virtual void on_update_application() override;
private:
	// SCENE OBJECTS
	entity floor = entity(*new cube(), LIT_SHADER_SOURCE);
	entity mid_air_platform = entity(*new cube(), LIT_SHADER_SOURCE);

	entity red_cube = entity(*new cube(), LIT_SHADER_SOURCE);
	entity blue_cube = entity(*new cube(), LIT_SHADER_SOURCE);

	// SCENE LIGHTS
	light* light_source;

	// SCENE CAMERAS
	perspective_camera* camera;
private:
	rendering_layer* m_rendering_layer;
	physics_layer* m_physics_layer = nullptr;

	double mouse_delta_x = 0.0;
	double mouse_delta_y = 180.0;

	glm::vec3 movement_vector = glm::vec3(0, 1.5, 15);
};