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

	const std::shared_ptr<renderer>& get_scene_renderer() const { return scene_renderer; }
private:
	void draw_depth_pass();
	void draw_render_pass();
private:
	const glm::vec4& skybox_color = glm::vec4(0.329f, 0.608f, 0.722f, 1);

	std::shared_ptr<renderer> scene_renderer;

	perspective_camera* scene_camera;
	light* main_light;

	std::vector<entity*> objects_on_scene;
private:
	frame_buffer* m_fbo;

	application& app = application::get();
};