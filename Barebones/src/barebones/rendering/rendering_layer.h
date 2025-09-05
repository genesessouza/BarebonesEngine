#pragma once

#include "renderer.h"
#include "light.h"

#include "barebones/core/layer.h"
#include "barebones/core/barebones/entity/entity.h"

class rendering_layer : public layer
{
public:
	rendering_layer(perspective_camera& camera);

	void on_update(timestep delta_time);
	void add_object(entity& object_to_render, bool is_main_light);
	
	std::vector<entity*>& get_scene_objects() { return objects_on_scene; }

	const std::shared_ptr<renderer>& get_scene_renderer() const { return scene_renderer; }
private:
	const glm::vec4& skybox_color = glm::vec4(0.329f, 0.608f, 0.722f, 1);
	std::shared_ptr<renderer> scene_renderer;

	perspective_camera* scene_camera;
	std::vector<entity*> objects_on_scene;
	light* main_light;
};