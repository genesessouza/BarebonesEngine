#pragma once

#include "barebones/core/layer.h"
#include "barebones/core/barebones/camera/perspective_camera.h"

class event_layer : public layer
{
public:
	event_layer();

	static std::unique_ptr<event_layer> create();

	void on_update(timestep delta_time);
};
