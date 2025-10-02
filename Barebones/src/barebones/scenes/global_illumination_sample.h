#pragma once

//#include "barebones/core/entry.h"
#include "barebones/barebones.h"

class global_illumination_sample : public application
{
public:
	global_illumination_sample();
private:
	virtual void on_update_application() override;
private:
	// SCENE OBJECTS
	entity floor = entity(*new cube(), LIT_SHADER_SOURCE);

	entity front_wall = entity(*new cube(), LIT_SHADER_SOURCE);
	entity back_wall = entity(*new cube(), LIT_SHADER_SOURCE);
	entity left_wall = entity(*new cube(), LIT_SHADER_SOURCE);
	entity right_wall = entity(*new cube(), LIT_SHADER_SOURCE);
	
	entity mid_air_platform = entity(*new cube(), LIT_SHADER_SOURCE);

	entity red_cube = entity(*new cube(), LIT_SHADER_SOURCE);
	entity blue_cube = entity(*new cube(), LIT_SHADER_SOURCE);
	entity green_cube = entity(*new cube(), LIT_SHADER_SOURCE);

	// SCENE LIGHTS
	light* light_source;

	// SCENE CAMERAS
	perspective_camera* camera;
private:
	rendering_layer* m_rendering_layer;

	double mouse_delta_x = 0.0;
	double mouse_delta_y = 0.0;
};