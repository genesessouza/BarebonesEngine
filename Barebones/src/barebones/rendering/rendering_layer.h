#pragma once

#include "renderer.h"
#include "light.h"

#include "barebones/core/layer.h"
#include "barebones/core/barebones/entity/entity.h"

class rendering_layer : public layer
{
public:
	rendering_layer(perspective_camera& camera, const glm::vec4& skybox_color = glm::vec4(0.0f));

	void on_update(timestep delta_time);
	void add_object(entity_object& object_to_render, bool is_main_light);

	std::vector<entity_object*>& get_objects_on_scene() { return objects_on_scene; }

	//std::vector<gizmo>& get_objects_on_scene_gizmo() { return objects_on_scene_gizmo; }
	//std::vector<bounds>& get_objects_on_scene_bounds() { return objects_on_scene_bounds; }

	void set_show_objects_on_scene_gizmo(bool enabled) { show_objects_on_scene_gizmo = enabled; }
	void set_show_objects_on_scene_bounds(bool enabled) { show_objects_on_scene_bounds = enabled; }

	bool get_show_objects_on_scene_gizmo() const { return show_objects_on_scene_gizmo; }
	bool get_show_objects_on_scene_bounds() const { return show_objects_on_scene_bounds; }

	const std::shared_ptr<renderer>& get_scene_renderer() const { return scene_renderer; }
private:
	const glm::vec4& light_blue = glm::vec4(0.329f, 0.608f, 0.722f, 1);

	std::shared_ptr<renderer> scene_renderer;

	perspective_camera* scene_camera;
	light* main_light;

	std::vector<entity_object*> objects_on_scene;
	std::vector<gizmo> objects_on_scene_gizmo;
	std::vector<bounds> objects_on_scene_bounds;
private:
	bool show_objects_on_scene_gizmo;
	bool show_objects_on_scene_bounds;
};