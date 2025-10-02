#pragma once

#include "barebones/core/layer.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

class event_layer : public layer
{
public:
	event_layer();

	void update_aspect_ratio(perspective_camera* camera, int width, int height);
	void on_update(timestep delta_time);
};
