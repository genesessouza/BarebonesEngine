#pragma once

#include "barebones/barebones_src.h"
#include "barebones/core/application.h"

class sdf_raster_test : public application
{
public:
	sdf_raster_test();
private:
	virtual void on_update_application() override;
private:
	// SCENE OBJECTS
	entity quad1 = entity(new quad(), true);
	entity cube1 = entity(new cube(), true);

	// SCENE LIGHTS
	light* light_source;

	// SCENE CAMERAS
	perspective_camera* camera;
private:
	rendering_layer* m_rendering_layer;
private:
	float xPos = 0, yPos = 3, zPos = 7;
	float xRot = 0, yRot = 0, zRot = 0;
};