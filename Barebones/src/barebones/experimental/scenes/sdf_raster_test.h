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
	entity quad1 = entity(new quad(), SDF_INTEGRATION_SOURCE);
	entity cube1 = entity(new cube(), SDF_INTEGRATION_SOURCE);

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