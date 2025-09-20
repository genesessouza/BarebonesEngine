#pragma once

#include "renderer.h"

#include "barebones/core/layer.h"
#include "barebones/core/barebones/entity/entity.h"
#include "barebones/core/barebones/entity/light.h"
#include "core/frame_buffer.h"

class rendering_layer : public layer
{
public:
	rendering_layer(perspective_camera& camera);

	void add_object(entity& object_to_render, bool is_main_light);
	void on_update(timestep delta_time);

	std::vector<entity*>& get_objects_on_scene() { return objects_on_scene; }

	//std::vector<gizmo>& get_objects_on_scene_gizmo() { return objects_on_scene_gizmo; }
	//std::vector<bounds>& get_objects_on_scene_bounds() { return objects_on_scene_bounds; }

	void set_show_objects_on_scene_gizmo(bool enabled) { show_objects_on_scene_gizmo = enabled; }
	void set_show_objects_on_scene_bounds(bool enabled) { show_objects_on_scene_bounds = enabled; }

	bool get_show_objects_on_scene_gizmo() const { return show_objects_on_scene_gizmo; }
	bool get_show_objects_on_scene_bounds() const { return show_objects_on_scene_bounds; }

	const std::shared_ptr<renderer>& get_scene_renderer() const { return scene_renderer; }
private:
	void draw_depth_pass();
	void draw_render_pass();
	void draw_screen_quad();
private:
	const glm::vec4& skybox_color = glm::vec4(0.329f, 0.608f, 0.722f, 1);

	std::shared_ptr<renderer> scene_renderer;

	perspective_camera* scene_camera;
	light* main_light;

	std::vector<entity*> objects_on_scene;
	std::vector<gizmo*> objects_on_scene_gizmo;
	std::vector<bounds*> objects_on_scene_bounds;
private:
	bool show_objects_on_scene_gizmo;
	bool show_objects_on_scene_bounds;
private:
	frame_buffer* m_fbo;

	glm::mat4 light_view_matrix = glm::mat4(1.0f);
	glm::mat4 light_proj_matrix = glm::mat4(1.0f);
};