#pragma once

#include "renderer.h"
#include "light.h"

#include "barebones/core/layer.h"

class rendering_layer : public layer
{
public:
	rendering_layer(perspective_camera& camera);

	void on_update(timestep delta_time);
	void add_object(entity& object_to_render, bool is_main_light);

	std::vector<entity*>& get_scene_objects() { return objects_on_scene; }
	std::vector<gizmo>& get_objects_gizmo() { return objects_gizmo_on_scene; }
	std::vector<bounds>& get_objects_bounds() { return objects_bounds_on_scene; }

	void set_show_object_gizmo(bool enabled) { show_object_gizmo = enabled; }
	void set_show_object_bounds(bool enabled) { show_object_bounds = enabled; }

	bool get_show_object_gizmo() const { return show_object_gizmo; }
	bool get_show_object_bounds() const { return show_object_bounds; }

	const std::shared_ptr<renderer>& get_scene_renderer() const { return scene_renderer; }
private:
	const glm::vec4& skybox_color = glm::vec4(0.329f, 0.608f, 0.722f, 1); // light blue
	std::shared_ptr<renderer> scene_renderer;
private:
	bool show_object_gizmo;
	bool show_object_bounds;
private:
	perspective_camera* scene_camera;

	std::vector<entity*> objects_on_scene;
	std::vector<gizmo> objects_gizmo_on_scene;
	std::vector<bounds> objects_bounds_on_scene;

	light* main_light;
};