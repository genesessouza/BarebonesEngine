#pragma once

#include "barebones/rendering/renderer.h"
#include "barebones/core/layer.h"
#include "barebones/core/barebones/entity/entity.h"

#include "barebones/core/application.h"

class gizmos_layer : public layer
{
public:
	gizmos_layer(std::vector<entity*> scene_objs, perspective_camera& scene_cam);

	void on_update(timestep delta_time);
	void on_event(event& event);
private:
	perspective_camera* scene_cam;
	std::vector<entity*> objs_on_scene;
	std::vector<gizmo> gizmos_on_scene;
};