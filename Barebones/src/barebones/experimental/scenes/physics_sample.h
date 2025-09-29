#pragma once

#include "barebones/core/entry.h"
#include "barebones/core/application.h"

#include "barebones/core/barebones/entity/entity.h"
#include "barebones/core/barebones/entity/light.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

#include "barebones/rendering/rendering_layer.h"
#include "barebones/physics/physics_layer.h"

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
	physics_layer* m_physics_layer;

	double mouse_delta_x = 0.0;
	double mouse_delta_y = 180.0;

	glm::vec3 movement_vector = glm::vec3(0, 1.5, 15);
};