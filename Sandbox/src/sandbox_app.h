#pragma once

#include "barebones/barebones_src.h"

class app : 
	public application
{
public:
	app();
private:
	virtual void on_update_application() override;
private:
	// SCENE OBJECTS
	entity quad1 = entity(quad());
	entity cube1 = entity(cube());

	// SCENE LIGHTS
	light* light_source;

	// SCENE CAMERAS
	perspective_camera* camera;
private:
	rendering_layer* m_rendering_layer;
private:
	float xPos = 0, yPos = 0, zPos = 0;
	float xRot = 0, yRot = 0, zRot = 0;

	glm::vec3 sun = glm::vec3(1.0f, 0.9f, 0.7f);
};