#pragma once

#include "barebones/core/entry.h"
#include "barebones/core/application.h"
#include "barebones/core/barebones/camera/perspective_camera.h"
#include "barebones/core/barebones/entity/entity.h"

#include "barebones/rendering/rendering_layer.h"

class sdf_raster_test : public application
{
public:
	sdf_raster_test();
private:
	virtual void on_update_application() override;
private:
	// SCENE OBJECTS
	entity floor = entity(*new cube(), LIT_SHADER_SOURCE);
	entity back_wall = entity(*new cube(), LIT_SHADER_SOURCE);

	entity cube1 = entity(*new cube(), LIT_SHADER_SOURCE);
	entity cube2 = entity(*new cube(), LIT_SHADER_SOURCE);
	entity cube3 = entity(*new cube(), LIT_SHADER_SOURCE);

	// SCENE LIGHTS
	light* light_source;

	// SCENE CAMERAS
	perspective_camera* camera;
private:
	rendering_layer* m_rendering_layer;
};