#pragma once

#include "../../layer.h"

class event_layer : public layer
{
public:
	event_layer();

	void on_update(timestep delta_time);
};
