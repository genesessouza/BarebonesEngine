#pragma once

#include "barebones/core/layer.h"
#include "barebones/core/barebones/entity/entity.h"

class physics_layer : public layer
{
public:
	physics_layer();

	void add_object(entity& obj);
	void on_update(timestep delta_time);
private:
	std::vector<entity*> entities_on_scene;
};